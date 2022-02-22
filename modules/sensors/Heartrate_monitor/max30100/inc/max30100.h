 
#ifndef MAX30100_H_
#define MAX30100_H_

#include "crf.h"
#include "signals.h"
#include "sys_if.h"

/*
MAX30100 module
==========

0.0.1.

State Machine for MAX30100 heartrate monitor and blood oxygen sensor. 

Requirements for the module:
*/

/*
 * MAX30100 SIGNALS
 */

typedef enum max30100_signals_ten
{
    SIG_MAX30100 = SIGNAL_FROM_CLASS(SIG_CLASS_max30100),
    SIG_MAX30100_ONLINE,
    SIG_MAX30100_OFFLINE,
} max30100_signals_ten;

#define SIG_MAX30100_ONLINE_TYPE             max30100_status_tst
#define SIG_MAX30100_OFFLINE_TYPE            max30100_status_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct max30100_status_tst
{
} max30100_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct max30100_cfg_tst
{
} max30100_cfg_tst;

typedef struct
{
    /* PUBLIC */
    chsm_tst                             super;

    /* PRIVATE */
}max30100_tst;

chsm_result_ten max30100_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

bool max30100_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool max30100_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);

#define MAX30100_READ_PERIOD_VALUE     100

#define MAX30100_RETRY_TIMEOUT   500
#define MAX30100_MAX_ERROR_COUNT (((bodycontour_tst *)self)->config_st.max_error_cnt_u16)

#endif /* MAX30100_H_ */
