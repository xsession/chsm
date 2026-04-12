"""
VHDL state machine code generator for CHSM.

Usage:
    render.py <html_file> [--output=<file>] [--entity-name=<name>]

Generates a VHDL state machine entity from a CHSM drawing (.html) file.
"""
import sys
import os
import json
import jinja2
from pathlib import Path

# Add parent directory to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent))

from c_gen.hsm.sm_jinja import JinjaStateMachine


TEMPLATE_DIR = (Path(__file__).parent.parent / 'c_gen' / 'templates').resolve()
TEMPLATE_NAME = 'chsm_vhdl_template.jinja'


def load_model_from_html(html_path):
    """Extract the JSON model from a CHSM HTML drawing file."""
    import re
    with open(html_path, 'r') as f:
        content = f.read()
    m = re.search(r"<pre id='chsm-json-data'>\n(?P<json>.+)</pre>", content, re.DOTALL)
    if not m:
        raise ValueError(f'No JSON data found in {html_path}')
    return json.loads(m.group('json'))


def render(model, template_params=None):
    """Render a VHDL state machine from a CHSM model dict.

    Args:
        model: CHSM model dict (as parsed from the HTML drawing).
        template_params: Optional dict of template parameters
            (e.g. entity_name, clock_name, reset_name, event_width).

    Returns:
        Generated VHDL source code as a string.
    """
    sm = JinjaStateMachine(model)
    data = sm.data.copy()
    data['template_params'] = template_params or {}

    # Convert sets to tuples for template compatibility
    for key in list(data.keys()):
        if isinstance(data[key], set):
            data[key] = tuple(data[key])

    env = jinja2.Environment(
        loader=jinja2.FileSystemLoader(str(TEMPLATE_DIR)),
        trim_blocks=True,
        lstrip_blocks=True,
    )
    template = env.get_template(TEMPLATE_NAME)
    return template.render(data=data)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    html_file = sys.argv[1]
    output_file = None
    entity_name = 'state_machine'

    for arg in sys.argv[2:]:
        if arg.startswith('--output='):
            output_file = arg.split('=', 1)[1]
        elif arg.startswith('--entity-name='):
            entity_name = arg.split('=', 1)[1]

    model = load_model_from_html(html_file)
    code = render(model, template_params={'entity_name': entity_name})

    if output_file:
        with open(output_file, 'w') as f:
            f.write(code)
        print(f'Generated {output_file}')
    else:
        print(code)
