import sys
import os
import shutil

# Protocols
sys.path.insert(0, os.path.abspath('../../../languages/c/modules/protocols/i2c_master/doc'))
shutil.copyfile("../../../languages/c/modules/protocols/i2c_master/doc/i2c_master.html", "./_static/i2c_master.html")

sys.path.insert(0, os.path.abspath('../../../languages/c/modules/protocols/spi_master/doc'))
shutil.copyfile("../../../languages/c/modules/protocols/spi_master/doc/spi_master.html", "./_static/spi_master.html")

sys.path.insert(0, os.path.abspath('../../../languages/c/modules/protocols/canopen/doc'))
shutil.copyfile("../../../languages/c/modules/protocols/canopen/doc/canopen.html", "./_static/canopen.html")

# Devices
sys.path.insert(0, os.path.abspath('../../../languages/c/modules/devices/temperature/lm73/doc'))
shutil.copyfile("../../../languages/c/modules/devices/temperature/lm73/doc/lm73.html", "./_static/lm73.html")

sys.path.insert(0, os.path.abspath('../../../languages/c/modules/devices/temperature/tmp468/doc'))
shutil.copyfile("../../../languages/c/modules/devices/temperature/tmp468/doc/tmp468.html", "./_static/tmp468.html")



# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'CHSM'
copyright = '2023, Janos Szeman'
author = 'Janos Szeman'
release = '1.0.0.'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['m2r',]

templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
html_css_files = [
    'decorator.css',
]
