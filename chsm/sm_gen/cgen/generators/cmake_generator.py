import io
import os
import re
import sys
import logging
import json
import jinja2
import pkg_resources
from datetime import datetime
import collections
import pprint

class CmakeGen:
    def __init__(self):
        self.lib_linked_libs = 
        self.test_linked_libs = 
        pass
    
    def createFile(self, file_loc, file_name, content):
        file_dir = os.path.join(file_loc, file_name)
        try:
            if not os.path.exists(file_dir):
                fd_name = open(file_dir, "a+")
                fd_name.write(content)
                fd_name.close()  
        except OSError:
            pass

    def generate_cmake_files(self, cmake_path, module_name, cmake_target_mode_selector):
        template_lib = str(pkg_resources.resource_string(__name__, 'template/cmake_lib.jinja'), encoding='utf-8')
        template_test = str(pkg_resources.resource_string(__name__, 'template/cmake_test.jinja'), encoding='utf-8')
    
        tmpl_lib = jinja2.Template(template_lib, trim_blocks=True, lstrip_blocks=True)
        lib_str = tmpl_lib.render(lib_name = module_name, package_name = 'chsm', linked_libs = ('signal_classes_modules', ))
        tmpl_lib = jinja2.Template(template_lib, trim_blocks=True, lstrip_blocks=True)
        lib_str = tmpl_lib.render(lib_name = module_name, package_name = 'chsm', linked_libs = (''))
    
        if(cmake_target_mode_selector == "library"):
          self.createFile(os.path.join(str(cmake_path), str(module_name)),
                          "CMakeLists.txt", 
                          self.cmake_library_template.format(name=module_name) )
          
        self.createFile(os.path.join(str(cmake_path), str(module_name) + "/test"), 
                        "CMakeLists.txt", 
                        self.cmake_test_build_template.format(name=module_name) )
        return py_str
        
        
        
        
if __name__ == '__main__':
    
    c = CmakeGen()
    c.generate_cmake_files('C:\Users\Yoga\OneDrive\Desktop\GIT\paper_dispenser\fw\c\externals\chsm\chsm\sm_gen\cgen\generators\test', 'kecske1', 'library')