#ifndef {{cookiecutter.module_name|upper}}_FUNCTIONS_H
#define {{cookiecutter.module_name|upper}}_FUNCTIONS_H

#include "{{cookiecutter.module_name}}.h"
#include "chsm.h"
#include "cevent.h"
#include <stdbool.h>

void {{cookiecutter.module_name}}_10ms_callback(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_inc_error_counter(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_init(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_init_wait(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_read_id(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_reset_error_cnt(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_reset_pointer(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_reset_timer(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_set_full_powerdown(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_set_full_powerup(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_set_resolution(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_start_read(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_unplugged(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_update_temp(chsm_tst *self, const cevent_tst *e_pst);

void send_offline_event(chsm_tst *self, const cevent_tst *e_pst);

void send_online_event(chsm_tst *self, const cevent_tst *e_pst);

void {{cookiecutter.module_name}}_debug_log_func(chsm_tst *self, const cevent_tst *est, uint8_t *trans_name, const char *state_func);
extern char {{cookiecutter.module_name}}_debug_state_ac[20];

bool {{cookiecutter.module_name}}_id_match(chsm_tst *self, const cevent_tst *e_pst);

bool {{cookiecutter.module_name}}_wait_cnt(chsm_tst *self, const cevent_tst *e_pst);

typedef enum {{cookiecutter.module_name}}_state_id_ten
{
    S_READ_ID_REG = 4,
    S_UNPLUGGED = 5,
    S_SET_RESOLUTION = 6,
    S_RESET_PTR_REG = 7,
    S_IDLE = 8,
    S_READING = 9,
    S_POWER_DOWN = 10,
    S_POWER_UP = 11,
    S_WAIT_POWER_DOWN = 12,
    S_WAIT_POWER_UP = 13,
} {{cookiecutter.module_name}}_state_id_ten;


/*
Signals:
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
