#ifndef {{cookiecutter.module_name|upper}}_FUNCTIONS_H
#define {{cookiecutter.module_name|upper}}_FUNCTIONS_H

#include "chsm.h"
#include "cevent.h"
#include <stdbool.h>
{% for include in cookiecutter.includes %}
#include "{{include}}.h"
{% endfor %}

{% for function in cookiecutter.functions %}
void {{cookiecutter.module_name}}_{{function}}(chsm_tst *self, const cevent_tst *e_pst);
{% endfor %}

void {{cookiecutter.module_name}}_debug_log_func(chsm_tst *self, const cevent_tst *est, uint8_t *trans_name, const char *state_func);
extern char {{cookiecutter.module_name}}_debug_state_ac[20];

typedef enum {{cookiecutter.module_name}}_state_id_ten
{
{% for enum in cokkiecutter.state_enums %}
{{ enum }}
{% endfor %}
} {{cookiecutter.module_name}}_state_id_ten;

/*
Signals:
{% for  %}
    SIG_I2C_RESULT_ADDR_NACK 
    SIG_I2C_RESULT_DATA_NACK 
    SIG_I2C_RESULT_SUCCESS   
    SIG_{{cookiecutter.module_name|upper}}_READ            
    SIG_SYS_TICK_10ms        
*/

/*
Other function notes:

{{cookiecutter.module_name}}_error_count:

{{cookiecutter.module_name}}_timeout:
*/
#endif
