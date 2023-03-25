#ifndef TMP468_FUNCTIONS_H
#define TMP468_FUNCTIONS_H

/*Generated with CHSM v0.0.0 at 2021.11.05 09.26.08*/

#include "cevent.h"
#include "chsm.h"
#include "tmp468.h"
#include <stdbool.h>

void tmp468_10ms_callback(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_i2c_power_state(chsm_tst *self, const cevent_tst *e_pst);

/*Increase the error counter.*/
void tmp468_inc_error_counter(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_init(chsm_tst *self, const cevent_tst *e_pst);

/*Try to read the ID register from the TMP468 by sending a write-read transaction to the I2C
 * master.*/
void tmp468_read_id(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_read_sensor_status(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_read_temp(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_reset_status_timer(chsm_tst *self, const cevent_tst *e_pst);

/*Reset the timer counter.*/
void tmp468_reset_timer(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_send_offline_event(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_send_online_event(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_set_n_factor1(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_set_n_factor2(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_set_n_factor3(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_set_n_factor4(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_unlock_regs(chsm_tst *self, const cevent_tst *e_pst);

void tmp468_update_status(chsm_tst *self, const cevent_tst *e_pst);

/*- Update the temperature display
- Send an event with the new value
- Decrease the error counter*/
void tmp468_update_temp(chsm_tst *self, const cevent_tst *e_pst);

/*True, if the response data is equal to 0x190. See TMP468 datasheet section: 7.5.1.7 Identification
 * Register*/
bool tmp468_id_match(chsm_tst *self, const cevent_tst *e_pst);

typedef enum tmp468_state_id_ten
{
    S_TMP468             = 0,
    S_IDLE               = 1,
    S_READ_TEMP          = 2,
    S_READ_ID_REG        = 3,
    S_ONLINE             = 5,
    S_READ_SENSOR_STATUS = 4,
    S_SET_N_FACTOR       = 6,
    S_SET_N_FACTOR1      = 7,
    S_SET_N_FACTOR2      = 8,
    S_SET_N_FACTOR3      = 9,
    S_SET_N_FACTOR4      = 10,
    S_UNLOCK_REGS        = 11,
    S_INIT               = 12,
} tmp468_state_id_ten;

/*
Signals:
    SIG_I2C_RESULT_ADDR_NACK
    SIG_I2C_RESULT_DATA_NACK
    SIG_I2C_RESULT_SUCCESS
    SIG_SYS_TICK_10ms
    SIG_TMP468_I2C_POWER_STATE
*/

/*
Other function notes:

tmp468_error_count:
    Return true, if the error counter is greater or equal then the parameter.

tmp468_status_timeout:

tmp468_timeout:
    Evaluates to true if the internal timer counter is greater than or equal to the parameter.
*/
#endif
