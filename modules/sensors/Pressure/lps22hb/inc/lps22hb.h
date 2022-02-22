 
#ifndef LPS22HB_H_
#define LPS22HB_H_

#include "crf.h"
#include "signals.h"
#include "sys_if.h"

/*
LPS22HB module
==========

0.0.1.

State Machine for LPS22HB pressure sensor.

Requirements for the module:
*/

/*
 * LPS22HB SIGNALS
 */

typedef enum lps22hb_signals_ten
{
    SIG_LPS22HB = SIGNAL_FROM_CLASS(SIG_CLASS_lps22hb),
    SIG_LPS22HB_ONLINE,
    SIG_LPS22HB_OFFLINE,
} lps22hb_signals_ten;

#define SIG_LPS22HB_ONLINE_TYPE             lps22hb_status_tst
#define SIG_LPS22HB_OFFLINE_TYPE            lps22hb_status_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct lps22hb_status_tst
{
} lps22hb_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct lps22hb_cfg_tst
{
} lps22hb_cfg_tst;

typedef struct
{
    /* PUBLIC */
    chsm_tst                             super;

    /* PRIVATE */
}lps22hb_tst;

chsm_result_ten lps22hb_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

bool lps22hb_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool lps22hb_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);

#define LPS22HB_READ_PERIOD_VALUE     100

#define LPS22HB_RETRY_TIMEOUT   500
#define LPS22HB_MAX_ERROR_COUNT (((bodycontour_tst *)self)->config_st.max_error_cnt_u16)

#endif /* LPS22HB_H_ */
