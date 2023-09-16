
add_module_lib(
        NAME 
                {{cookiecutter.module_name}}
        PACKAGE
                chsm
        SOURCE
                src/{{cookiecutter.module_name}}.c
                src/{{cookiecutter.module_name}}_functions.c
        INCLUDE 
                inc
        LINK 
                {% if cookiecutter.comm_periph == "i2c" %}
                chsm::i2c_master
                {% elif cookiecutter.comm_periph == "spi" %}
                chsm::spi_master
                {% elif cookiecutter.comm_periph == "can" %}
                chsm::canopen
                {% else %}
                {% endif %}
                signal_classes_modules
        DEFINES 
                NDEBUG
        STANDARD
                11
)