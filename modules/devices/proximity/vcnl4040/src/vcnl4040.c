/*Generated with CHSM v0.0.0 at 2023.02.06 21.35.50*/
#include "cevent.h"
#include "chsm.h"
#include "vcnl4040.h"
#include "vcnl4040_functions.h"


static chsm_result_ten s_reading(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_read_id(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_config(chsm_tst *self, const cevent_tst  *e_pst);

static chsm_result_ten s_config(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            vcnl4040_10ms_callback(self, e_pst);
            break;
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_read_id(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_DATA_NACK:
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_SUCCESS:
            send_online_event(self, e_pst);
            vcnl4040_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        case SIG_SYS_TICK_10ms:
            vcnl4040_10ms_callback(self, e_pst);
            break;
    }

    if(vcnl4040_timeout(self, e_pst, VCNL4040_RETRY_TIMEOUT))
    {
        vcnl4040_reset_timer(self, e_pst);
        return chsm_transition(self, s_unplugged);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            send_online_event(self, e_pst);
            vcnl4040_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        case SIG_SYS_TICK_10ms:
            vcnl4040_10ms_callback(self, e_pst);
            break;
    }

    if(vcnl4040_timeout(self, e_pst, VCNL4040_UNPLUGGED_TIMEOUT))
    {
        vcnl4040_reset_timer(self, e_pst);
        vcnl4040_reset_timer(self, e_pst);
        vcnl4040_read_id(self, e_pst);
        return chsm_transition(self, s_read_id);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_VCNL4040_READ:
            vcnl4040_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        case SIG_SYS_TICK_10ms:
            vcnl4040_10ms_callback(self, e_pst);
            break;
    }

    if(vcnl4040_timeout(self, e_pst, VCNL4040_READ_PERIOD))
    {
        vcnl4040_reset_timeout(self, e_pst);
        vcnl4040_start_read(self, e_pst);
        return chsm_transition(self, s_reading);
    }

    if(vcnl4040_error_cnt(self, e_pst, VCNL4040_MAX_ERROR_CNT))
    {
        send_offline_event(self, e_pst);
        vcnl4040_reset_timer(self, e_pst);
        vcnl4040_read_id(self, e_pst);
        return chsm_transition(self, s_read_id);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_reading(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            vcnl4040_inc_error_counter(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_DATA_NACK:
            vcnl4040_inc_error_counter(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_SUCCESS:
            vcnl4040_update_data(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_10ms:
            vcnl4040_10ms_callback(self, e_pst);
            break;
    }

    if(vcnl4040_timeout(self, e_pst, VCNL4040_READ_PERIOD))
    {
        vcnl4040_inc_error_counter(self, e_pst);
        vcnl4040_reset_timer(self, e_pst);
        vcnl4040_reset_timer(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    if(vcnl4040_error_cnt(self, e_pst, VCNL4040_MAX_ERROR_CNT))
    {
        vcnl4040_reset_timer(self, e_pst);
        send_offline_event(self, e_pst);
        vcnl4040_reset_timer(self, e_pst);
        vcnl4040_read_id(self, e_pst);
        return chsm_transition(self, s_read_id);
    }

    return chsm_ignored(self);
}

chsm_result_ten (None, None)(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            vcnl4040_init(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_read_id(self, e_pst);
            return chsm_transition(self, s_read_id);
    }

    return chsm_ignored(self);
}
