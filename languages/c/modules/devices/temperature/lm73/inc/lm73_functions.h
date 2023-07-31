#ifndef LM73_FUNCTIONS_H
#define LM73_FUNCTIONS_H

/*Generated with CHSM v0.0.0 at 2023.07.25 10.48.28*/

#include "lm73.h"
#include "chsm.h"
#include "cevent.h"
#include <stdbool.h>


void lm73_10ms_callback(chsm_tst *self, const cevent_tst *e_pst);

void lm73_inc_error_counter(chsm_tst *self, const cevent_tst *e_pst);

void lm73_init(chsm_tst *self, const cevent_tst *e_pst);

void lm73_init_wait(chsm_tst *self, const cevent_tst *e_pst);

void lm73_read_id(chsm_tst *self, const cevent_tst *e_pst);

void lm73_reset_error_cnt(chsm_tst *self, const cevent_tst *e_pst);

void lm73_reset_pointer(chsm_tst *self, const cevent_tst *e_pst);

void lm73_reset_timer(chsm_tst *self, const cevent_tst *e_pst);

void lm73_set_full_powerdown(chsm_tst *self, const cevent_tst *e_pst);

void lm73_set_full_powerup(chsm_tst *self, const cevent_tst *e_pst);

void lm73_set_resolution(chsm_tst *self, const cevent_tst *e_pst);

void lm73_start_read(chsm_tst *self, const cevent_tst *e_pst);

void lm73_unplugged(chsm_tst *self, const cevent_tst *e_pst);

void lm73_update_temp(chsm_tst *self, const cevent_tst *e_pst);

void send_offline_event(chsm_tst *self, const cevent_tst *e_pst);

void send_online_event(chsm_tst *self, const cevent_tst *e_pst);

void lm73_debug_log_func(chsm_tst *self, const cevent_tst *est, uint8_t *trans_name, const char *state_func);
extern char lm73_debug_state_ac[20];

bool lm73_id_match(chsm_tst *self, const cevent_tst *e_pst);

bool lm73_wait_cnt(chsm_tst *self, const cevent_tst *e_pst);

typedef enum lm73_state_id_ten
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
} lm73_state_id_ten;


/*
Signals:
    SIG_I2C_RESULT_ADDR_NACK 
    SIG_I2C_RESULT_DATA_NACK 
    SIG_I2C_RESULT_SUCCESS   
    SIG_LM73_READ            
    SIG_SYS_TICK_10ms        
*/

/*
Other function notes:

lm73_error_count:

lm73_timeout:
*/
#endif
