#ifndef VCNL4040_FUNCTIONS_H
#define VCNL4040_FUNCTIONS_H

/*Generated with CHSM v0.0.0 at 2023.07.02 22.34.14*/

#include "vcnl4040.h"
#include "chsm.h"
#include "cevent.h"
#include <stdbool.h>


void send_offline_event(chsm_tst *self, const cevent_tst *e_pst);

void send_online_event(chsm_tst *self, const cevent_tst *e_pst);

void vcnl4040_10ms_callback(chsm_tst *self, const cevent_tst *e_pst);

void vcnl4040_inc_error_counter(chsm_tst *self, const cevent_tst *e_pst);

void vcnl4040_init(chsm_tst *self, const cevent_tst *e_pst);

void vcnl4040_read_id(chsm_tst *self, const cevent_tst *e_pst);

void vcnl4040_reset_timeout(chsm_tst *self, const cevent_tst *e_pst);

void vcnl4040_reset_timer(chsm_tst *self, const cevent_tst *e_pst);

void vcnl4040_start_read(chsm_tst *self, const cevent_tst *e_pst);

void vcnl4040_update_data(chsm_tst *self, const cevent_tst *e_pst);

void vcnl4040_debug_log_func(chsm_tst *self, const cevent_tst *est, uint8_t *trans_name, const char *state_func);
extern char vcnl4040_debug_state_ac[20];

typedef enum vcnl4040_state_id_ten
{
    S_CONFIG = 3,
    S_READ_ID = 4,
    S_UNPLUGGED = 5,
    S_IDLE = 6,
    S_READING = 7,
} vcnl4040_state_id_ten;


/*
Signals:
    SIG_I2C_RESULT_ADDR_NACK 
    SIG_I2C_RESULT_DATA_NACK 
    SIG_I2C_RESULT_SUCCESS   
    SIG_SYS_TICK_10ms        
*/

/*
Other function notes:

vcnl4040_error_cnt:

vcnl4040_timeout:
*/
#endif
