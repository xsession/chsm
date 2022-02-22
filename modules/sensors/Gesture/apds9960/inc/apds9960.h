 
#ifndef APDS9960_H_
#define APDS9960_H_

#include "crf.h"
#include "signals.h"
#include "sys_if.h"

/*
APDS9960 module
==========

0.0.1.

State Machine for APDS9960 optical gesture sensor and light component detector. 

Requirements for the module:
*/

/*
 * APDS9960 SIGNALS
 */

typedef enum apds9960_signals_ten
{
    SIG_APDS9960 = SIGNAL_FROM_CLASS(SIG_CLASS_apds9960),
    SIG_APDS9960_ONLINE,
    SIG_APDS9960_OFFLINE,
} apds9960_signals_ten;

#define SIG_APDS9960_ONLINE_TYPE             apds9960_status_tst
#define SIG_APDS9960_OFFLINE_TYPE            apds9960_status_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct apds9960_status_tst
{
} apds9960_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct apds9960_cfg_tst
{
} apds9960_cfg_tst;

typedef struct
{
    /* PUBLIC */
    chsm_tst                             super;

    /* PRIVATE */
}apds9960_tst;

chsm_result_ten apds9960_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

bool apds9960_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool apds9960_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);

#define APDS9960_READ_PERIOD_VALUE     100

#define APDS9960_RETRY_TIMEOUT   500
#define APDS9960_MAX_ERROR_COUNT (((bodycontour_tst *)self)->config_st.max_error_cnt_u16)

#endif /* APDS9960_H_ */
