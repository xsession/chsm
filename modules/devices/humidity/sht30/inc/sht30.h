#ifndef SHT30_H_
#define SHT30_H_

#include "crf.h"
#include "i2c_driver_if.h"
#include "sys_if.h"
#include "i2c_master.h"
#include "sht30_regs.h"
#include "signal_classes_modules.h"

/*
SHT30 driver
==========

This module implements a high level driver for the  sensor.

Requirements for the SHT30 module:

*/

/*
 * SHT30 SIGNALS
 */

typedef enum sht30_signals_ten
{
    SIG_SHT30_READ_SAMPLE = SIGNAL_FROM_CLASS(SIG_CLASS_SHT30),
    SIG_SHT30_READ_STATUS,
    SIG_SHT30_ONLINE,
    SIG_SHT30_OFFLINE,
    SIG_SHT30_I2C_POWER_STATE,
} sht30_signals_ten;

#define SIG_SHT30_ONLINE_TYPE          sht30_status_tst
#define SIG_SHT30_OFFLINE_TYPE         sht30_status_tst
#define SIG_SHT30_READ_STATUS_TYPE     sht30_status_tst
#define SIG_SHT30_READ_SAMPLE_TYPE     sht30_sample_tst
#define SIG_SHT30_I2C_POWER_STATE_TYPE sht30_power_state_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct sht30_power_state_tst
{
    cevent_tst super; // Signal and GC stuff
    uint16_t   power_state_on_u16;
} sht30_power_state_tst;

typedef struct sht30_sample_tst
{
    cevent_tst super;        // Signal and GC stuff
    int32_t    humidity_i32; // Humidity
    int32_t    temp_C_i32;   // Temperature
} sht30_sample_tst;

typedef struct sht30_status_tst
{
    cevent_tst           super; // Signal and GC stuff
    sht30_status_reg_tun status_tun;
    uint16_t             id_u16; // Sensor ID
} sht30_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct sht30_cfg_tst
{
    uint16_t id_u16;            // This ID will be inserted into temperature events
    uint16_t period_ms_u16;     // Temperature read period in ms
    uint16_t max_error_cnt_u16; // Number of times the module is allowed to fail a read operation
                                // before going offline
    uint8_t  address_u8;        // I2C slave address of the sensor
    uint32_t reading_timeout;
} sht30_cfg_tst;

typedef struct
{
    uint32_t *serialNumber;
} sht30_serial_num_tst;

typedef struct sht30_st sht30_tst;

struct sht30_st
{
    /* PUBLIC */
    chsm_tst             super;
    sht30_cfg_tst        config_st;
    int32_t              temp_C_i32;
    int32_t              humidity_i32;
    sht30_status_tst     status_st;
    sht30_serial_num_tst serial_num;
    bool                 valid_b;
    uint8_t              state_id_en;

    /* PRIVATE */
    uint32_t counter_u32;
    uint16_t error_counter_u32;
    uint32_t reading_timeout_counter;
    uint16_t write_count;

    i2c_transaction_tst t_st;
    uint8_t             tx_buff_au8[10];
    uint8_t             rx_buff_au8[10];
};

chsm_result_ten sht30_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

bool sht30_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool sht30_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);

#define SHT30_READ_PERIOD_VALUE     (100UL)
#define SHT30_MAX_ERROR_COUNT_VALUE (5UL)
#define SHT30_CONVERSION_TIME       (20UL)
#define SHT30_UNPLUG_RETRY          (5000UL)

#define SHT30_RETRY_TIMEOUT   (500UL)
#define SHT30_READ_PERIOD     (((sht30_tst *)self)->config_st.period_ms_u16)
#define SHT30_MAX_ERROR_COUNT (((sht30_tst *)self)->config_st.max_error_cnt_u16)

#endif /* SHT30_H_ */
