# -*- coding: utf-8 -*-
# CHSM Documentation — Sphinx Configuration
# ===========================================

import os
import sys
from datetime import datetime

# -- Path setup ---------------------------------------------------------------
sys.path.insert(0, os.path.abspath("../../gui"))

# -- Project information ------------------------------------------------------
project = "CHSM"
author = "Janos Szeman"
copyright = f"2019–{datetime.now().year}, {author}"
release = "1.0.0"
version = "1.0"

# -- General configuration ----------------------------------------------------
extensions = [
    "myst_parser",              # Markdown support (replaces m2r)
    "sphinx.ext.autodoc",       # Python API docs
    "sphinx.ext.viewcode",      # [source] links
    "sphinx.ext.intersphinx",   # Cross-project references
    "sphinx_copybutton",        # Copy button on code blocks
    "sphinx_design",            # Cards, tabs, grids
    "sphinxcontrib.mermaid",    # Mermaid diagrams
]

templates_path = ["_templates"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]
source_suffix = {".rst": "restructuredtext", ".md": "markdown"}
master_doc = "index"

# -- MyST configuration -------------------------------------------------------
myst_enable_extensions = [
    "colon_fence",
    "deflist",
    "fieldlist",
    "tasklist",
]
myst_heading_anchors = 3

# -- Options for HTML output ---------------------------------------------------
html_theme = "sphinx_rtd_theme"
html_theme_options = {
    "logo_only": False,
    "navigation_depth": 4,
    "collapse_navigation": False,
    "sticky_navigation": True,
    "style_nav_header_background": "#2b2b2b",
}
html_static_path = ["_static"]
html_css_files = ["custom.css"]
html_title = "CHSM Documentation"
html_short_title = "CHSM"

# -- Options for LaTeX output --------------------------------------------------
latex_engine = "xelatex"
latex_documents = [
    (master_doc, "chsm.tex", "CHSM — Hierarchical State Machine Framework",
     author, "manual"),
]
latex_elements = {
    "papersize": "a4paper",
    "pointsize": "11pt",
    "preamble": r"""
\usepackage{enumitem}
\setlistdepth{9}
""",
}

# -- Intersphinx ---------------------------------------------------------------
intersphinx_mapping = {
    "python": ("https://docs.python.org/3", None),
}

# -- Mermaid --------------------------------------------------------------------
mermaid_output_format = "raw"
