#ifndef TMP468_H_
#define TMP468_H_

#include "crf.h"
#include "i2c_driver_if.h"
#include "sys_if.h"
#include "i2c_master.h"
#include "tmp468_regs.h"
#include "signal_classes_modules.h"

/*
TMP468 driver
==========

This module implements a high level driver for the TMP468 temperature sensor.

Requirements for the TMP468 module:
    * Read out the ID register to test the communication
    * Retry the ID register reading after a timeout if the previous read failed
    * Send an online event, when the link is established
    * Send an offline event if the temperature read operation fails for a number of times
    * Send periodic temperature events
    * Allow triggered temperature reads
    * Add a user defined ID into the events (so the application can differentiate between
      the different TMP468 events)
*/

/*
 * TMP468 SIGNALS
 */

typedef enum tmp468_signals_ten
{
    SIG_TMP468_TEMP = SIGNAL_FROM_CLASS(SIG_CLASS_TMP468),
    SIG_TMP468_ONLINE,
    SIG_TMP468_OFFLINE,
    SIG_TMP468_READ_TEMP,
    SIG_TMP468_AVAILABLE,
    SIG_TMP468_READ_SENSOR_STATUS,
    SIG_TMP468_READ_SUCCESS,
    SIG_TMP468_I2C_POWER_STATE,
} tmp468_signals_ten;

#define SIG_TMP468_TEMP_TYPE               tmp468_temp_tst
#define SIG_TMP468_ONLINE_TYPE             tmp468_status_tst
#define SIG_TMP468_OFFLINE_TYPE            tmp468_status_tst
#define SIG_TMP468_READ_TEMP_TYPE          tmp468_temp_tst
#define SIG_TMP468_READ_SENSOR_STATUS_TYPE tmp468_status_tst
#define SIG_TMP468_AVAILABLE_TYPE          cevent_tst
#define SIG_TMP468_READ_SUCCESS_TYPE       tmp468_temp_tst
#define SIG_TMP468_I2C_POWER_STATE_TYPE    tmp468_power_state_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct tmp468_power_state_tst
{
    cevent_tst super; // Signal and GC stuff
    uint16_t   power_state_on_u16;
} tmp468_power_state_tst;

typedef struct tmp468_temp_tst
{
    cevent_tst super;         // Signal and GC stuff
    int16_t    temp_C_i32[5]; // Temperature
    uint16_t   id_u16;
} tmp468_temp_tst;

typedef struct tmp468_status_tst
{
    cevent_tst                           super; // Signal and GC stuff
    tmp468_remote_ch_open_status_reg_tun status_tun;
    uint16_t                             id_u16;
} tmp468_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct tmp468_cfg_tst
{
    uint16_t id_u16;             // This ID will be inserted into temperature events
    uint16_t temp_period_ms_u16; // Temperature read period in ms
    uint16_t max_error_cnt_u16;  // Number of times the module is allowed to fail a read operation
                                 // before going offline
    uint8_t address_u8;          // I2C slave address of the sensor
} tmp468_cfg_tst;

typedef struct
{
    /* PUBLIC */
    chsm_tst                             super;
    tmp468_cfg_tst                       config_st;
    int16_t                              temp_C_i32[5];
    tmp468_remote_ch_open_status_reg_tun status_tun;
    bool                                 valid_b;
    uint8_t                              state_id_en;

    /* PRIVATE */
    uint32_t counter_status_u32;
    uint32_t counter_u32;
    uint16_t error_counter_u32;
    uint16_t reg_inc_counter_u16;

    i2c_transaction_tst t_st;
    uint8_t             tx_buff_au8[10];
    uint8_t             rx_buff_au8[10];
} tmp468_tst;

chsm_result_ten tmp468_top(chsm_tst *self, const cevent_tst *e_pst);

bool tmp468_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool tmp468_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);
void tmp468_set_state(chsm_tst *self, const cevent_tst *e_pst, uint16_t state_id);

#define TMP468_READ_PERIOD_VALUE     100
#define TMP468_STATUS_READ_PERIOD    5000
#define TMP468_MAX_ERROR_COUNT_VALUE 5
#define TMP468_READ_REG_NUM          4

#define TMP468_RETRY_TIMEOUT   500
#define TMP468_READ_PERIOD     (((tmp468_tst *)self)->config_st.temp_period_ms_u16)
#define TMP468_MAX_ERROR_COUNT (((tmp468_tst *)self)->config_st.max_error_cnt_u16)

#endif /* TMP468_H_ */
