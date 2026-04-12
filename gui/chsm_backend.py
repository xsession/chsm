"""
chsm_backend

Usage:
    chsm_backend.py [options] [FILE]

Options:
    -s, --server-only     Do not open the application with Chrome app mode just wait for clients at http://127.0.0.1:8000/main.html
    -c, --code-gen        Generate code and quit. Don't start the GUI.
    -o, --output-lang    Set witch language will you generate the output. (c, python, javascript, java)
"""
import re
from time import time
import eel
import os
import sys
import json
from pathlib import Path
from docopt import docopt
import logging
from pprint import pprint
import collections
import subprocess
import tkinter as tk
from tkinter.filedialog import askopenfilename, asksaveasfilename
from c_gen import StateMachine
import module_generator
import random
import threading
import socket
import select
import gevent

def resource_path(relative_path):
    """ Get absolute path to resource, works for dev and for PyInstaller """
    try:
        # PyInstaller creates a temp folder and stores path in _MEIPASS
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")

    return os.path.join(base_path, relative_path)

class HtmlException(Exception):
    pass

TOP_STATE_NAME = r'chsm_result_ten\s+(?P<top_func>\w+)\(chsm_tst\s+\*self,\s+const\s+cevent_tst\s+\*e_pst)\s*;'
random_port = random.randint(1,65535)
class ChsmException(Exception):
    pass

def save_html(html_fname: Path, drawing: str, json_data: str):
        backend_path =     (Path(__file__).parent).absolute().resolve()
        template_dir =     (backend_path / 'c_gen' / 'templates').absolute().resolve()
        template_html =    template_dir / 'template.html'
        drawing_js =       template_dir / 'wheel.js'
        web_dir =          (backend_path / 'web').absolute().resolve()
        drawing_css =      web_dir / 'drawing.css'
        theme_css =      web_dir / 'themes' / 'default.css'


        if not template_html.exists():
            logging.error(f'File not found: {template_html}')
            eel.send_event('SAVE_RESULT', {
                'result': False,
                'file': f'{template_html}',
                'message': f'File not found: {template_html}',
                })
            return

        if not drawing_js.exists():
            logging.error(f'File not found: {drawing_js}')
            eel.send_event('SAVE_RESULT', {
                'result': False,
                'file': f'{drawing_js}',
                'message': f'File not found: {drawing_js}',
                })
            return

        if not drawing_css.exists():
            logging.error(f'File not found: {drawing_css}')
            eel.send_event('SAVE_RESULT', {
                'result': False,
                'file': f'{drawing_css}',
                'message': f'File not found: {drawing_css}',
                })
            return
        
        if not theme_css.exists():
            logging.error(f'File not found: {theme_css}')
            eel.send_event('SAVE_RESULT', {
                'result': False,
                'file': f'{theme_css}',
                'message': f'File not found: {theme_css}',
                })
            return

        with open(template_html, 'r') as tmp_html, \
            open(drawing_css, 'r') as drw_css, \
            open(theme_css, 'r') as theme_css_, \
            open(drawing_js, 'r') as drw_js, \
            open(html_fname, 'w') as html:
            template = tmp_html.read()
            css = drw_css.read()
            js = drw_js.read()
            theme_style = theme_css_.read()
            output = template.format(style=css, theme_style=theme_style ,drawing=drawing, json_data=json_data, script=js)
            html.write(output)
            logging.info(f'Saved drawing in {html_fname}')
            eel.send_event('SAVE_RESULT', {
                'result': True,
                'filepath': f'{html_fname}',
                'filename': html_fname.name,
                'message': f'Saved drawing in {html_fname}',
                })

def open_html(html_path):
    with open(html_path, 'r') as html:
        content = html.read()
        m = re.search(r"\<pre id='chsm-json-data'\>\n(?P<json>.+)\<\/pre\>", content, re.DOTALL)
        if not m:
            logging.error(f'No JSON data was found in file: {html_path}')
            return None
        try:
            return json.loads(m.group('json'))
        except json.JSONDecodeError as e:
            logging.error(f'JSON syntax error in html file {str(html_path)}. \nError message: {str(e)}')
            return None

class Project:
    def __init__(self, h_file_path=None):
        self.backend_path =     (Path(__file__).parent).absolute().resolve()
        self.template_dir =     (self.backend_path / 'c_gen' / 'templates').absolute().resolve()

        self.h_file_path =      Path(h_file_path) if h_file_path else self._open_header_dialog()

        logging.info(f'Opening project for {self.h_file_path}')

        if not self.h_file_path.exists():
            logging.error("Selected file {self.h_file_path} doesn't exists.")
            raise ChsmException("Selected file {self.h_file_path} doesn't exists.")

        self.default_config =   self._load_config_from_file(self.template_dir / 'settings.json')
        self.user_config =      self._load_user_config(self.h_file_path)
        self.file_config =      self.user_config.get(self.h_file_path.name, {})

        self.c_templates =      self._get_cfg('templates')

        self.src_dir =          self._get_cfg('src_dir')
        self.doc_dir =          self._get_cfg('doc_dir')

        logging.info(f'src dir: {self.src_dir}, doc dir: {self.doc_dir}')

        self.c_file_path =      self._get_c_path(self.h_file_path)
        self.func_h_path =      self._get_func_h_path(self.h_file_path)
        self.html_file_path =   self._get_html_path(self.h_file_path)

        logging.info(f'Outputs: c => {self.c_file_path.name}, h => {self.func_h_path.name}, html => {self.html_file_path.name}')

        self.model_json =       self.template_dir / 'model.json'

        if not self.model_json.exists():
            logging.error(f'File not found: {self.model_json}')
            return

        self.model = self._get_model(self.html_file_path)

        #pprint(self.model, indent=4)

    def _get_c_path(self, hpath):
        c_name = self.file_config.get('machine_c', hpath.stem + '.c')
        return (hpath.parent / self.src_dir / c_name).absolute().resolve()

    def _get_html_path(self, hpath):
        html_name = self.file_config.get('machine_html', hpath.stem + '.html')
        return (hpath.parent / self.doc_dir / html_name).absolute().resolve()

    def _get_func_h_path(self, hpath):
        h_name = self.file_config.get('func_h', hpath.stem + '_functions.h')
        return (hpath.parent / h_name).absolute().resolve()

    def _new_model(self):
        with open(self.h_file_path, 'r') as h:
            h_content = h.read()

            m = re.search(TOP_STATE_NAME, h_content)
            if m:
                self.model['states']['__top__']['title'] = m.group('top_func')
            else:
                logging.error(f'Could not find a valid top state handler function declaration in file {self.h_file_path}')
                self.model['states']['__top__']['title'] = f'{self.h_file_path.stem}__top__'

        return self.model

    def _get_default_model(self):
        with open(self.model_json, 'r') as model_file:
            return json.load(model_file)

    def _get_model(self, html_path):
        if not html_path.exists():
            logging.warning(f'File not found: {html_path}\n Creating new model.')
            return self._get_default_model()

        with open(html_path, 'r') as html:
            content = html.read()
            m = re.search(r"\<pre id='chsm-json-data'\>\n(?P<json>.+)\<\/pre\>", content, re.DOTALL)
            if not m:
                logging.error(f'No JSON data was found in file: {html_path}')
                return self._get_default_model()
            try:
                return json.loads(m.group('json'))
            except json.JSONDecodeError as e:
                logging.error(f'JSON syntax error in html file {str(html_path)}. \nError message: {str(e)}')
                return self._get_default_model()

    def _get_cfg(self, key):
        return self.user_config.get(key, self.default_config[key])

    def _load_config_from_file(self, json_path):
        with open(json_path, 'r') as cfg_file:
            return json.load(cfg_file)
        
    def _find_file(self, dir_path, f_name):
        
        for parent in dir_path.parents:
            p = parent / f'.chsm/{f_name}'
            print(f'Searching settings in {p}')
            if p.exists():
                self.cfg_file_path = p
                return p

        return None

    def _find_user_config_file(self, h_dir, h_name):
        print(f'_find_user_config_file({h_dir}, {h_name})')
        if h_name:
            f_name = self._find_file(h_dir, f'{h_name}.json')
            if f_name:
                return f_name
            
        return self._find_file(h_dir, 'settings.json')

    def _load_user_config(self, hpath):
        user_cfg_path = self._find_user_config_file(hpath.parent, hpath.stem)

        if user_cfg_path == None:
            return {}

        return self._load_config_from_file(user_cfg_path)

    def _open_header_dialog(self):
        root = tk.Tk()
        root.attributes("-topmost", True)
        root.withdraw()
        filepath = askopenfilename(title='Open state mechine declaration', filetypes=(('C header file', '.h'), ('State chart', '.html')))
        logging.info(f'User selected path: {filepath}')
        return Path(filepath)

    def save_html(self, drawing: str, json_data: str):
        self.model = json.loads(json_data)
        save_html(self.html_file_path, drawing, json_data)

    def generate_code(self):
        sm = StateMachine(self.model, self.h_file_path, self.func_h_path.name, self.c_templates, self.file_config)

        with open(self.c_file_path, 'w') as cfile:
            logging.info(f'Generating code into file: {self.c_file_path}')
            cfile.write(str(sm.ast))

        with open(self.func_h_path, 'w') as hfile:
            logging.info(f'Generating code into file: {self.func_h_path}')
            hfile.write(str(sm.h_ast))

    def print_repository_info(self, repo):
        print('Repository description: {}'.format(repo.description))
        print('Repository active branch is {}'.format(repo.active_branch))

        for remote in repo.remotes:
            print('Remote named "{}" with URL "{}"'.format(remote, remote.url))

        print('Last commit for repository is {}.'.format(str(repo.head.commit.hexsha)))
        print('pickpack version name : {}'.format(repo.active_branch).format(str(repo.head.commit.hexsha)))
project = None

# ---------------------------------------------------------------------------
# Debug channel – TCP server that accepts JSON-line messages from a running
# state machine and forwards them to the GUI via Eel.
#
# Protocol (one JSON object per line, terminated with \n):
#   {"type": "state",     "active": ["state_title_1", "state_title_2"]}
#   {"type": "variables", "vars": {"counter": 5, "flag": true}}
#   {"type": "event",     "name": "EVT_START", "data": "optional payload"}
#   {"type": "reset"}
# ---------------------------------------------------------------------------

class DebugServer:
    def __init__(self):
        self._server_socket = None
        self._client = None
        self._thread = None
        self._running = False
        self.port = 0
        self.log = []          # last N events for the event-log panel
        self.MAX_LOG = 200

    @property
    def connected(self):
        return self._client is not None

    def start(self, port):
        if self._running:
            self.stop()

        self.port = port
        self._running = True
        self._thread = threading.Thread(target=self._serve, daemon=True)
        self._thread.start()
        logging.info(f'Debug server started on port {self.port}')

    def stop(self):
        self._running = False
        if self._client:
            try:
                self._client.close()
            except Exception:
                pass
            self._client = None
        if self._server_socket:
            try:
                self._server_socket.close()
            except Exception:
                pass
            self._server_socket = None
        logging.info('Debug server stopped')

    def _serve(self):
        try:
            self._server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self._server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self._server_socket.bind(('127.0.0.1', self.port))
            self._server_socket.listen(1)
            self._server_socket.settimeout(1.0)
        except OSError as e:
            logging.error(f'Debug server bind failed: {e}')
            eel.send_event('DEBUG_STATUS', {'connected': False, 'error': str(e)})
            self._running = False
            return

        eel.send_event('DEBUG_STATUS', {'connected': False, 'listening': True, 'port': self.port})

        while self._running:
            # Accept loop
            try:
                ready, _, _ = select.select([self._server_socket], [], [], 1.0)
                if not ready:
                    continue
                client, addr = self._server_socket.accept()
            except (OSError, socket.timeout):
                continue

            self._client = client
            self._client.settimeout(1.0)
            logging.info(f'Debug client connected from {addr}')
            eel.send_event('DEBUG_STATUS', {'connected': True, 'port': self.port})

            buf = ''
            while self._running and self._client:
                try:
                    data = self._client.recv(4096)
                    if not data:
                        break
                    buf += data.decode('utf-8', errors='replace')
                    while '\n' in buf:
                        line, buf = buf.split('\n', 1)
                        line = line.strip()
                        if not line:
                            continue
                        try:
                            msg = json.loads(line)
                            self._handle_message(msg)
                        except json.JSONDecodeError as e:
                            logging.warning(f'Debug bad JSON: {e}')
                except socket.timeout:
                    continue
                except (ConnectionResetError, OSError):
                    break

            self._client = None
            logging.info('Debug client disconnected')
            eel.send_event('DEBUG_STATUS', {'connected': False, 'listening': True, 'port': self.port})
            eel.send_event('DEBUG_UPDATE', {'type': 'reset'})

    def _handle_message(self, msg):
        msg_type = msg.get('type', '')

        if msg_type == 'event':
            if len(self.log) >= self.MAX_LOG:
                self.log.pop(0)
            self.log.append(msg)

        eel.send_event('DEBUG_UPDATE', msg)

    def send_to_target(self, msg):
        if self._client:
            try:
                self._client.sendall((json.dumps(msg) + '\n').encode('utf-8'))
            except (OSError, BrokenPipeError) as e:
                logging.warning(f'Debug send failed: {e}')


debug_server = DebugServer()


@eel.expose
def debug_start(port):
    """Start the debug TCP server on the given port."""
    try:
        port = int(port)
    except (ValueError, TypeError):
        port = 9999
    debug_server.start(port)


@eel.expose
def debug_stop():
    """Stop the debug TCP server."""
    debug_server.stop()
    eel.send_event('DEBUG_STATUS', {'connected': False, 'listening': False})


@eel.expose
def debug_send(msg):
    """Send a JSON command to the connected target (e.g. step, continue)."""
    debug_server.send_to_target(msg)

@eel.expose
def save_state_machine(drawing: str, json_data: str, filepath: str):
    if project:
        project.save_html(drawing, json_data)
    elif filepath:
        save_html(Path(filepath), drawing, json_data)
    else:
        root = tk.Tk()
        root.attributes("-topmost", True)
        root.withdraw()
        filepath = asksaveasfilename(title='Save state mechine drawing', filetypes=(('State chart', '.html'),))
        logging.info(f'User selected path: {filepath}')
        save_html(Path(filepath), drawing, json_data)

@eel.expose
def open_window():
    python_path = os.environ['PYTHON_PATH']
    print("Python environment variable:")
    print(python_path)
    subprocess.Popen([ python_path + "/python.exe","./gui/chsm_backend.py"], shell=True)

@eel.expose
def open_file():
    root = tk.Tk()
    root.attributes("-topmost", True)
    root.withdraw()
    filepath = askopenfilename(title='Open state mechine declaration', filetypes=(('C header file', '.h'), ('State chart', '.html')))
    if not filepath:
        logging.info(f'File open canceled by user')
        return

    logging.info(f'User selected path: {filepath}')

    if filepath.endswith('.h'):
        global project
        project = Project(filepath)
        eel.load_json(json.dumps(project.model), Path(filepath).name, filepath)

    elif filepath.endswith('.html'):
        model = open_html(filepath)
        if model:
            eel.load_json(json.dumps(model), Path(filepath).name, filepath)

@eel.expose
def genereate_code():
    project.generate_code()

@eel.expose
def exit_program():
    quit()

@eel.expose
def startup():
    if project:
        eel.load_json(json.dumps(project.model), Path(args['FILE']).name, args['FILE'])
 
def exit_program_(page, sockets):
    print('exit_program') 
    eel.start('main.html', port=random_port, mode='None', block=False, close_callback=exit_program_)

def monitor_power_events_windows(port_number):
    import win32api
    import win32con

    def on_power_broadcast(hwnd, msg, wparam, lparam):
        if wparam == win32con.PBT_APMRESUMEAUTOMATIC:  # System wakes up from sleep
            eel.start('main.html', port=random_port, mode='None', block=False, close_callback=exit_program_)

    win32api.SetConsoleCtrlHandler(on_power_broadcast, 1)
   
        
if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO, format='%(asctime)s %(filename)-20s:%(lineno)-4s %(message)s')
    args = docopt(__doc__)
    print(args)

    if args['--output-lang']:
        pass

    if args['FILE']:
        p = Path(args['FILE'])
        if p.exists():
            project = Project(p)
            if args['--code-gen']:
                project.generate_code()
                quit()

    eel.init((Path(__file__).parent / 'web').absolute().resolve())
    
    if args['--server-only']:
        eel.start('main.html', mode=None, port=random_port)
    else:
        eel.start('main.html', port=random_port, mode='None')
        # eel.start('main.html', port=random_port, mode='None', block=False, close_callback=exit_program_)
        # gevent.get_hub().join()
    # try:
    #     while True:
    #         print("Waiting for power event")
    #         monitor_power_events_windows(random_port)
    #         eel.start('main.html', port=random_port, mode='None', block=False, close_callback=exit_program_)
    # except KeyboardInterrupt:
    #     print('KeyboardInterrupt')

