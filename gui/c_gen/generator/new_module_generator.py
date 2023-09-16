import eel
import tkinter as tk
from tkinter.filedialog import askopenfilename, asksaveasfilename, askdirectory
from pathlib import Path
from docopt import docopt
import logging
import cookiecutter

@eel.expose
def generate_module(name : str, 
                    version : str, 
                    description : str,
                    module_location : str, 
                    linked_libs : str,
                    package_name : str,
                    comm_periph : str,
                    address : str,
                    author : str,
                    full_name : str,
                    licence : str,
                    ):
    print("Generate module!")
    print(name)
    print(module_location)
    template_path = (Path(__file__).parent / 'module_template' / '_template_new').absolute().resolve() 
    cookiecutter( template_path, 
                 extra_context={
                        "module_name": name,
                        "version": version,
                        "description": description,
                        "module_location": module_location,
                        "module_linked_libs": linked_libs,
                        "module_package_name": package_name,
                        "module_functions": "",
                        "module_includes": "",
                        "comm_periph": comm_periph,
                        "address": address,
                        "author": author,
                        "full_name": full_name,
                        "licence": licence
                 },
                 output_dir=module_location
                 )
    eel.successful_generate()

@eel.expose
def browse_dir_path():
    root = tk.Tk()
    root.attributes("-topmost", True)
    root.withdraw()
    filepath = askdirectory()
    if not filepath:
        logging.info(f'File open canceled by user')
        return
    logging.info(f'User selected path: {filepath}')
    path = Path(filepath).parent
    print(path)
    eel.set_choosen_path_to_input_text(filepath)

@eel.expose
def create_project():
    eel.start('new_module/new_module.html', port=0, mode='None') 
