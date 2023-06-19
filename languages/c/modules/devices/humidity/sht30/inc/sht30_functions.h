#ifndef SHT30_FUNCTIONS_H
#define SHT30_FUNCTIONS_H

/*Generated with CHSM v0.0.0 at 2023.06.19 13.34.39*/

#include "sht30.h"
#include "chsm.h"
#include "cevent.h"
#include <stdbool.h>


void sht30_10ms_callback(chsm_tst *self, const cevent_tst *e_pst);

void sht30_check_status(chsm_tst *self, const cevent_tst *e_pst);

/*W 0x3041*/
void sht30_clear_status_reg(chsm_tst *self, const cevent_tst *e_pst);

/*W 0xF23D*/
void sht30_get_status(chsm_tst *self, const cevent_tst *e_pst);

void sht30_i2c_power_state(chsm_tst *self, const cevent_tst *e_pst);

/*Increase the error counter.*/
void sht30_inc_error_counter(chsm_tst *self, const cevent_tst *e_pst);

void sht30_init(chsm_tst *self, const cevent_tst *e_pst);

/*W 0x2C10*/
void sht30_init_measurement_reg_write(chsm_tst *self, const cevent_tst *e_pst);

/*WR 0x2C10*/
void sht30_measurement_reg_read(chsm_tst *self, const cevent_tst *e_pst);

void sht30_measurement_reg_write(chsm_tst *self, const cevent_tst *e_pst);

/*Reset the timer counter.*/
void sht30_reset_timer(chsm_tst *self, const cevent_tst *e_pst);

void sht30_send_offline_event(chsm_tst *self, const cevent_tst *e_pst);

/*wefwsefwe*/
void sht30_send_online_event(chsm_tst *self, const cevent_tst *e_pst);

void sht30_unplugged(chsm_tst *self, const cevent_tst *e_pst);

void sht30_update_data(chsm_tst *self, const cevent_tst *e_pst);

void sht30_debug_log_func(chsm_tst *self, const cevent_tst *est, uint8_t *trans_name, const char *state_func);
extern char sht30_debug_state_ac[20];

typedef enum sht30_state_id_ten
{
    S_CHECK_STATUS = 5,
    S_MEASUREMENT_INIT_WRITE = 8,
    S_MEASUREMENT_INIT_READ = 9,
    S_UNPLUGGED = 10,
    S_CLEAR_STATUS_REG = 11,
    S_IDLE = 2,
    S_MEASUREMENT_WRITE = 3,
} sht30_state_id_ten;


/*
Signals:
    SIG_I2C_RESULT_ADDR_NACK  
    SIG_I2C_RESULT_DATA_NACK  
    SIG_I2C_RESULT_SUCCESS    
    SIG_SHT30_I2C_POWER_STATE 
    SIG_SYS_TICK_10ms         
*/

/*
Other function notes:

sht30_error_count:
    Return true, if the error counter is greater or equal then the parameter.

sht30_timeout:
    Evaluates to true if the internal timer counter is greater than or equal to the parameter.
*/
#endif
