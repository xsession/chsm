 
#ifndef LSM9DS1_H_
#define LSM9DS1_H_

#include "crf.h"
#include "signals.h"
#include "sys_if.h"

/*
LSM9DS1 module
==========

0.0.1.

State Machine for LSM9DS1 inercia system sensor with maneto meter and gyroscope in it.

Requirements for the module:
*/

/*
 * LSM9DS1 SIGNALS
 */

typedef enum lsm9ds1_signals_ten
{
    SIG_LSM9DS1 = SIGNAL_FROM_CLASS(SIG_CLASS_lsm9ds1),
    SIG_LSM9DS1_ONLINE,
    SIG_LSM9DS1_OFFLINE,
} lsm9ds1_signals_ten;

#define SIG_LSM9DS1_ONLINE_TYPE             lsm9ds1_status_tst
#define SIG_LSM9DS1_OFFLINE_TYPE            lsm9ds1_status_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct lsm9ds1_status_tst
{
} lsm9ds1_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct lsm9ds1_cfg_tst
{
} lsm9ds1_cfg_tst;

typedef struct
{
    /* PUBLIC */
    chsm_tst                             super;

    /* PRIVATE */
}lsm9ds1_tst;

chsm_result_ten lsm9ds1_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

bool lsm9ds1_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool lsm9ds1_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);

#define LSM9DS1_READ_PERIOD_VALUE     100

#define LSM9DS1_RETRY_TIMEOUT   500
#define LSM9DS1_MAX_ERROR_COUNT (((bodycontour_tst *)self)->config_st.max_error_cnt_u16)

#endif /* LSM9DS1_H_ */
