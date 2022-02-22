 
#ifndef CCS811_H_
#define CCS811_H_

#include "crf.h"
#include "signals.h"
#include "sys_if.h"

/*
CCS811 module
==========

0.0.1.

State Machine for CCS811 air CO2 sensor. 

Requirements for the module:
*/

/*
 * CCS811 SIGNALS
 */

typedef enum ccs811_signals_ten
{
    SIG_CCS811 = SIGNAL_FROM_CLASS(SIG_CLASS_ccs811),
    SIG_CCS811_ONLINE,
    SIG_CCS811_OFFLINE,
} ccs811_signals_ten;

#define SIG_CCS811_ONLINE_TYPE             ccs811_status_tst
#define SIG_CCS811_OFFLINE_TYPE            ccs811_status_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct ccs811_status_tst
{
} ccs811_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct ccs811_cfg_tst
{
} ccs811_cfg_tst;

typedef struct
{
    /* PUBLIC */
    chsm_tst                             super;

    /* PRIVATE */
}ccs811_tst;

chsm_result_ten ccs811_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

bool ccs811_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool ccs811_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);

#define CCS811_READ_PERIOD_VALUE     100

#define CCS811_RETRY_TIMEOUT   500
#define CCS811_MAX_ERROR_COUNT (((bodycontour_tst *)self)->config_st.max_error_cnt_u16)

#endif /* CCS811_H_ */
