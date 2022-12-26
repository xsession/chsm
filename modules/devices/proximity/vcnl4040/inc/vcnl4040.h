 
#ifndef VCNL4040_H_
#define VCNL4040_H_

#include "crf.h"
#include "signals.h"
#include "sys_if.h"

/*
VCNL4040 module
==========

0.0.1.

It is a high resolution proximity and ambient light sensor.

Requirements for the module:
*/

/*
 * VCNL4040 SIGNALS
 */

typedef enum vcnl4040_signals_ten
{
    SIG_VCNL4040 = SIGNAL_FROM_CLASS(SIG_CLASS_vcnl4040),
    SIG_VCNL4040_ONLINE,
    SIG_VCNL4040_OFFLINE,
} vcnl4040_signals_ten;

#define SIG_VCNL4040_ONLINE_TYPE             vcnl4040_status_tst
#define SIG_VCNL4040_OFFLINE_TYPE            vcnl4040_status_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct vcnl4040_status_tst
{
} vcnl4040_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct vcnl4040_cfg_tst
{
} vcnl4040_cfg_tst;

typedef struct
{
    /* PUBLIC */
    chsm_tst                             super;

    /* PRIVATE */
}vcnl4040_tst;

chsm_result_ten vcnl4040_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

bool vcnl4040_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool vcnl4040_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);

#define VCNL4040_READ_PERIOD_VALUE     100

#define VCNL4040_RETRY_TIMEOUT   500
#define VCNL4040_MAX_ERROR_COUNT (((bodycontour_tst *)self)->config_st.max_error_cnt_u16)

#endif /* VCNL4040_H_ */
