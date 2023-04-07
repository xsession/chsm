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
        self.internal_lib_linked_libs = ['signal_classes_modules']
        self.external_lib_linked_libs = ['signal_classes_ext_modules']
        self.module_name = ''
        self.test_linked_libs = ['unity', self.module_name]
    
    def createFile(self, file_loc, file_name, content):
        file_dir = os.path.join(file_loc, file_name)
        try:
            if not os.path.exists(file_dir):
                fd_name = open(file_dir, "a+")
                fd_name.write(content)
                fd_name.close()  
        except OSError:
            pass

    def generate_cmake_files(self, cmake_path, module_name, package_name, linked_libs, test_linked_list, external_lib):
        self.module_name = module_name
        linked_element_str = ''
        
        template_lib = str(pkg_resources.resource_string(__name__, 'module_template/cmake_lib.jinja'), encoding='utf-8')
        # template_test = str(pkg_resources.resource_string(__name__, 'module_template/cmake_test.jinja'), encoding='utf-8')
    
        tmpl_lib = jinja2.Template(template_lib, trim_blocks=True, lstrip_blocks=True)
        if external_lib == True:
            self.external_lib_linked_libs.append(linked_libs)
            print(self.external_lib_linked_libs)
            for linked_element in self.external_lib_linked_libs:
                linked_element_str += f'{linked_element} \n\t\t'
            lib_str = tmpl_lib.render(lib_name = module_name, package_name = package_name, linked_libs = linked_element_str)
            print(lib_str)
        else:
            self.internal_lib_linked_libs.append(linked_libs)
            print(self.internal_lib_linked_libs)
            for linked_element in self.internal_lib_linked_libs:
                linked_element_str += f'{linked_element} \n\t\t'
            lib_str = tmpl_lib.render(lib_name = module_name, package_name = package_name, linked_libs = linked_element_str)
            print(lib_str)

        self.createFile(file_loc=os.path.join(str(cmake_path), str(module_name)), file_name="CMakeLists.txt", content=lib_str )
          
        # self.createFile(os.path.join(str(cmake_path), str(module_name) + "/test"), 
        #                 "CMakeLists.txt", 
        #                 test_str )
        
# if __name__ == '__main__':
    
#     c = CmakeGen()
#     c.generate_cmake_files(cmake_path='kecske', module_name='kecske1', package_name='chsm', linked_libs='i2c_master', test_linked_list='i2c_drv_mock', external_lib=True)