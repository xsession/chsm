#ifndef I2C_MASTER_FUNCTIONS_H
#define I2C_MASTER_FUNCTIONS_H

/*Generated with CHSM v0.0.0 at 2023.05.30 14.39.00*/

#include "i2c_master.h"
#include "chsm.h"
#include "cevent.h"
#include <stdbool.h>


void clear_transaction_info(chsm_tst *self, const cevent_tst *e_pst);

void i2c_1ms_callback(chsm_tst *self, const cevent_tst *e_pst);

void i2c_master_inc_dev_addr(chsm_tst *self, const cevent_tst *e_pst);

void i2c_master_inc_success_dev_addr(chsm_tst *self, const cevent_tst *e_pst);

void i2c_master_init(chsm_tst *self, const cevent_tst *e_pst);

void i2c_master_send_fail_response(chsm_tst *self, const cevent_tst *e_pst);

void i2c_master_send_success_response(chsm_tst *self, const cevent_tst *e_pst);

void i2c_master_start_rx(chsm_tst *self, const cevent_tst *e_pst);

void i2c_master_start_tx(chsm_tst *self, const cevent_tst *e_pst);

void i2c_master_stop(chsm_tst *self, const cevent_tst *e_pst);

void i2c_scan_start_step(chsm_tst *self, const cevent_tst *e_pst);

void scan_init(chsm_tst *self, const cevent_tst *e_pst);

void store_transaction_info(chsm_tst *self, const cevent_tst *e_pst);

void i2c_master_debug_log_func(chsm_tst *self, const cevent_tst *est, uint8_t *trans_name, const char *state_func);


bool device_addr_max_cnt(chsm_tst *self, const cevent_tst *e_pst);

typedef enum i2c_master_state_id_ten
{
    S_IDLE = 0,
    S_WRITE = 2,
    S_READ = 3,
    S_WR_WRITE = 5,
    S_WR_READ = 6,
    S_SCAN_IDLE = 9,
    S_SCAN_WRITE = 11,
} i2c_master_state_id_ten;


/*
Signals:
    SIG_I2C_ADDRS_RELEASE  
    SIG_I2C_BUS_SCAN       
    SIG_I2C_READ_FAIL      
    SIG_I2C_READ_SUCCESS   
    SIG_I2C_R_TRANSACTION  
    SIG_I2C_WRITE_FAIL     
    SIG_I2C_WRITE_SUCCESS  
    SIG_I2C_WR_TRANSACTION 
    SIG_I2C_W_TRANSACTION  
    SIG_SYS_TICK_1ms       
*/

/*
Other function notes:
*/
#endif
