import eel
import tkinter as tk
from tkinter.filedialog import askdirectory
from pathlib import Path
import logging
from cookiecutter.main import cookiecutter
import traceback
import webbrowser

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
    logging.info(f"Generate module: name={name}, location={module_location}")
    try:
        template_path = str((Path(__file__).parent / '..' / 'c_gen' / 'module_template' / '_template_new').absolute().resolve()) 
        logging.info(f"Template path: {template_path}")
        
        if not Path(template_path).exists():
            raise FileNotFoundError(f"Template directory not found: {template_path}")
        
        if not module_location or not module_location.strip():
            raise ValueError("Module location is required")
        
        output_dir = Path(module_location)
        if not output_dir.exists():
            raise FileNotFoundError(f"Output directory does not exist: {module_location}")

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
                     output_dir=str(module_location),
                     no_input=True
                     )
        logging.info(f"Module '{name}' generated successfully at {module_location}")
        eel.successful_generate()
    except Exception as e:
        logging.error(f"Module generation failed: {e}")
        traceback.print_exc()
        eel.failed_generate(str(e))

@eel.expose
def browse_dir_path():
    root = tk.Tk()
    root.attributes("-topmost", True)
    root.withdraw()
    filepath = askdirectory()
    root.destroy()
    if not filepath:
        logging.info('Directory selection canceled by user')
        return
    logging.info(f'User selected path: {filepath}')
    eel.set_choosen_path_to_input_text(filepath)

@eel.expose
def create_project():
    """Open the New Module dialog in a new browser tab/window."""
    # eel.show() opens a new page served by the already-running Eel server.
    # Unlike eel.start(), it does not block or create a new server instance.
    eel.show('new_module/new_module.html')
