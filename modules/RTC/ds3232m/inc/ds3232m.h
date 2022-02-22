 
#ifndef DS3232M_H_
#define DS3232M_H_

#include "crf.h"
#include "signals.h"
#include "sys_if.h"

/*
DS3232M module
==========

0.0.1.

State Machine for DS3232M MEMS based +-2ppm precies RTC.

Requirements for the module:
*/

/*
 * DS3232M SIGNALS
 */

typedef enum ds3232m_signals_ten
{
    SIG_DS3232M = SIGNAL_FROM_CLASS(SIG_CLASS_ds3232m),
    SIG_DS3232M_ONLINE,
    SIG_DS3232M_OFFLINE,
} ds3232m_signals_ten;

#define SIG_DS3232M_ONLINE_TYPE             ds3232m_status_tst
#define SIG_DS3232M_OFFLINE_TYPE            ds3232m_status_tst

/*
 * EVENT DEFINITIONS
 */

typedef struct ds3232m_status_tst
{
} ds3232m_status_tst;

/*
 * ACTIVE OBJECT
 */

typedef struct ds3232m_cfg_tst
{
} ds3232m_cfg_tst;

typedef struct
{
    /* PUBLIC */
    chsm_tst                             super;

    /* PRIVATE */
}ds3232m_tst;

chsm_result_ten ds3232m_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

bool ds3232m_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32);
bool ds3232m_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16);

#define DS3232M_READ_PERIOD_VALUE     100

#define DS3232M_RETRY_TIMEOUT   500
#define DS3232M_MAX_ERROR_COUNT (((bodycontour_tst *)self)->config_st.max_error_cnt_u16)

#endif /* DS3232M_H_ */
