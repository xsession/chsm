#ifndef {{cookiecutter.module_name|upper}}_H
#define {{cookiecutter.module_name|upper}}_H

#include "crf.h"
#include "sys_if.h"
#include "i2c_driver_if.h"
#include "i2c_master.h"
#include "signal_classes_modules.h"

/*
{{cookiecutter.module_name|upper}} driver
==========

This module implements a high level driver for the {{cookiecutter.module_name|upper}} temperature sensor.

Requirements for the {{cookiecutter.module_name|upper}} module:
    * Read out the ID register to test the communication
    * Retry the ID register reading after a timeout if the previous read failed
    * Send an online event, when the link is established
    * Send an offline event if the temperature read operation fails for a number of times
    * Send periodic temperature events
    * Allow triggered temperature reads
    * Add a user defined ID into the events (so the application can differentiate between
      the different {{cookiecutter.module_name|upper}} events)
*/

/*
 * {{cookiecutter.module_name|upper}} SIGNALS
 */

typedef enum {{cookiecutter.module_name}}_signals_ten
{
    SIG_{{cookiecutter.module_name|upper}}_TEMP = SIGNAL_FROM_CLASS(SIG_CLASS_{{cookiecutter.module_name|upper}}),
    SIG_{{cookiecutter.module_name|upper}}_ONLINE,
    SIG_{{cookiecutter.module_name|upper}}_OFFLINE,
    SIG_{{cookiecutter.module_name|upper}}_READ
} {{cookiecutter.module_name}}_signals_ten;

#define SIG_{{cookiecutter.module_name|upper}}_TEMP_TYPE      {{cookiecutter.module_name}}_temp_tst
#define SIG_{{cookiecutter.module_name|upper}}_ONLINE_TYPE    {{cookiecutter.module_name}}_status_tst
#define SIG_{{cookiecutter.module_name|upper}}_OFFLINE_TYPE   {{cookiecutter.module_name}}_status_tst
#define SIG_{{cookiecutter.module_name|upper}}_READ_TYPE      cevent_tst


/*
 * EVENT DEFINITIONS
 */

typedef struct {{cookiecutter.module_name}}_temp_tst
{
    cevent_tst          super;              // Signal and GC stuff
    int32_t             temp_C_i32;         // Temperature
    uint16_t            id_u16;             // Sensor ID
} {{cookiecutter.module_name}}_temp_tst;

typedef struct {{cookiecutter.module_name}}_status_tst
{
    cevent_tst          super;              // Signal and GC stuff
    uint16_t            id_u16;             // Sensor ID
} {{cookiecutter.module_name}}_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct {{cookiecutter.module_name}}_cfg_tst
{
    uint16_t            id_u16;             // This ID will be inserted into temperature events
    uint16_t            period_ms_u16;      // Temperature read period in ms
    uint16_t            max_error_cnt_u16;  // Number of times the module is allowed to fail a read operation before going offline
    uint8_t             address_u8;         // I2C slave address of the sensor
} {{cookiecutter.module_name}}_cfg_tst;

typedef struct {{cookiecutter.module_name}}_tst {{cookiecutter.module_name}}_tst;

struct {{cookiecutter.module_name}}_tst
{
    /* PUBLIC */
    chsm_tst            super;
    {{cookiecutter.module_name}}_cfg_tst        config_st;
    int16_t             temp_C_i32;
    bool                valid_b;
    uint16_t            resolution_u16;

    /* PRIVATE */
    uint32_t            counter_u32;
    uint16_t            error_counter_u32;
    uint16_t            wait_cnt_u16;

    i2c_transaction_tst t_st;
    uint8_t             tx_buff_au8[4];
    uint8_t             rx_buff_au8[4];
};

chsm_result_ten {{cookiecutter.module_name}}_top(chsm_tst *self, const cevent_tst  *e_pst);

bool {{cookiecutter.module_name}}_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool {{cookiecutter.module_name}}_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);


#define {{cookiecutter.module_name|upper}}_READ_PERIOD_VALUE      (100UL)
#define {{cookiecutter.module_name|upper}}_MAX_ERROR_COUNT_VALUE  (20UL)
#define {{cookiecutter.module_name|upper}}_RETRY_TIMEOUT          (500UL)
#define {{cookiecutter.module_name|upper}}_UNPLUGGED_TIMEOUT      (2000UL)
#define {{cookiecutter.module_name|upper}}_ID_REG_VALUE           (0x190)
#define {{cookiecutter.module_name|upper}}_WAIT_CNT               (12UL)

#define {{cookiecutter.module_name|upper}}_READ_PERIOD        ((({{cookiecutter.module_name}}_tst *)self)->config_st.period_ms_u16)
#define {{cookiecutter.module_name|upper}}_MAX_ERROR_COUNT    ((({{cookiecutter.module_name}}_tst *)self)->config_st.max_error_cnt_u16)

#endif
