 
#ifndef HTS221_H_
#define HTS221_H_

#include "crf.h"
#include "signals.h"
#include "sys_if.h"

/*
HTS221 module
==========

0.0.1.

State Machine for HTS221 humidity and temperature sensor.

Requirements for the module:
*/

/*
 * HTS221 SIGNALS
 */

typedef enum hts221_signals_ten
{
    SIG_HTS221 = SIGNAL_FROM_CLASS(SIG_CLASS_hts221),
    SIG_HTS221_ONLINE,
    SIG_HTS221_OFFLINE,
} hts221_signals_ten;

#define SIG_HTS221_ONLINE_TYPE             hts221_status_tst
#define SIG_HTS221_OFFLINE_TYPE            hts221_status_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct hts221_status_tst
{
} hts221_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct hts221_cfg_tst
{
} hts221_cfg_tst;

typedef struct
{
    /* PUBLIC */
    chsm_tst                             super;

    /* PRIVATE */
}hts221_tst;

chsm_result_ten hts221_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

bool hts221_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool hts221_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);

#define HTS221_READ_PERIOD_VALUE     100

#define HTS221_RETRY_TIMEOUT   500
#define HTS221_MAX_ERROR_COUNT (((bodycontour_tst *)self)->config_st.max_error_cnt_u16)

#endif /* HTS221_H_ */
