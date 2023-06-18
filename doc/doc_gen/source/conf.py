import sys
import os
import shutil

sys.path.insert(0, os.path.abspath('../../../languages\c\modules\protocols\i2c_master\doc'))



source_path = "../../../languages\c\modules\protocols\i2c_master\doc/i2c_master.html"
destination_path = "./_static/i2c_master.html"

# copy the source file to the destination
shutil.copyfile(source_path, destination_path)


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
