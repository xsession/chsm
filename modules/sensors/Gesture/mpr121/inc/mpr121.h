 
#ifndef MPR121_H_
#define MPR121_H_

#include "crf.h"
#include "signals.h"
#include "sys_if.h"

/*
MPR121 module
==========

0.0.1.

State Machine for MPR121 capacitive 12 channel touch sensor. 

Requirements for the module:
*/

/*
 * MPR121 SIGNALS
 */

typedef enum mpr121_signals_ten
{
    SIG_MPR121 = SIGNAL_FROM_CLASS(SIG_CLASS_mpr121),
    SIG_MPR121_ONLINE,
    SIG_MPR121_OFFLINE,
} mpr121_signals_ten;

#define SIG_MPR121_ONLINE_TYPE             mpr121_status_tst
#define SIG_MPR121_OFFLINE_TYPE            mpr121_status_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct mpr121_status_tst
{
} mpr121_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct mpr121_cfg_tst
{
} mpr121_cfg_tst;

typedef struct
{
    /* PUBLIC */
    chsm_tst                             super;

    /* PRIVATE */
}mpr121_tst;

chsm_result_ten mpr121_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

bool mpr121_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool mpr121_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);

#define MPR121_READ_PERIOD_VALUE     100

#define MPR121_RETRY_TIMEOUT   500
#define MPR121_MAX_ERROR_COUNT (((bodycontour_tst *)self)->config_st.max_error_cnt_u16)

#endif /* MPR121_H_ */
