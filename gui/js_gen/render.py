from jinja2 import Environment, FileSystemLoader

# Create the Jinja environment
env = Environment(loader=FileSystemLoader("templates"))

# Load the state machine template
state_machine_template = env.get_template("state_machine_template.js.j2")

# Render the state machine template with the state data and signals
state_machine_code = state_machine_template.render(state_data=state_data, signals=signals)

# Load the unit test template
unit_test_template = env.get_template("unit_test_template.js.j2")

# Render the unit test template with the test cases
unit_test_code = unit_test_template.render(test_cases=test_cases)

# Write the rendered code to files
with open("state_machine.js", "w") as file:
    file.write(state_machine_code)

with open("unit_test.js", "w") as file:
    file.write(unit_test_code)
