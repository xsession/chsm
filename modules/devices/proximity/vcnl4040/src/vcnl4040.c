/*Generated with CHSM v0.0.0 at 2022.12.10 21.29.01*/
#include "cevent.h"
#include "chsm.h"
#include "vcnl4040.h"
#include "vcnl4040_functions.h"


static chsm_result_ten s_reading(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_read_id(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_config(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_online(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_init(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_vcnl4040(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);

static chsm_result_ten s_vcnl4040(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case SIG_SYS_TIICK_10ms:
            vcnl4040_10ms_callback(self, e_pst);
            break;

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, vcnl4040_top, NULL, guards_only_b);
}

static chsm_result_ten s_init(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            chsm_exit_children(self, e_pst, ctx_pst);
            send_online_event(self, e_pst);
            vcnl4040_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        default:
            guards_only_b = false;
    }


    return chsm_handle_in_parent(self, ctx_pst, s_vcnl4040, NULL, guards_only_b);
}

static chsm_result_ten s_online(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {

        default:
            guards_only_b = false;
    }

    if(vcnl4040_error_cnt(self, e_pst, VCNL4040_MAX_ERROR_CNT))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        send_offline_event(self, e_pst);
        vcnl4040_reset_timer(self, e_pst);
        vcnl4040_read_id(self, e_pst);
        return chsm_transition(self, s_read_id);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_vcnl4040, send_offline_event, guards_only_b);
}

static chsm_result_ten s_config(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, s_vcnl4040, NULL, guards_only_b);
}

static chsm_result_ten s_read_id(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            chsm_exit_children(self, e_pst, ctx_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_DATA_NACK:
            chsm_exit_children(self, e_pst, ctx_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        default:
            guards_only_b = false;
    }

    if(vcnl4040_timeout(self, e_pst, VCNL4040_RETRY_TIMEOUT))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        vcnl4040_reset_timer(self, e_pst);
        return chsm_transition(self, s_unplugged);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_init, NULL, guards_only_b);
}

static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {

        default:
            guards_only_b = false;
    }

    if(vcnl4040_timeout(self, e_pst, VCNL4040_UNPLUGGED_TIMEOUT))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        vcnl4040_reset_timer(self, e_pst);
        vcnl4040_reset_timer(self, e_pst);
        vcnl4040_read_id(self, e_pst);
        return chsm_transition(self, s_read_id);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_init, NULL, guards_only_b);
}

static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case SIG_VCNL4040_READ:
            chsm_exit_children(self, e_pst, ctx_pst);
            vcnl4040_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        default:
            guards_only_b = false;
    }

    if(vcnl4040_timeout(self, e_pst, VCNL4040_READ_PERIOD))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        vcnl4040_reset_timeout(self, e_pst);
        vcnl4040_start_read(self, e_pst);
        return chsm_transition(self, s_reading);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_online, NULL, guards_only_b);
}

static chsm_result_ten s_reading(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            chsm_exit_children(self, e_pst, ctx_pst);
            vcnl4040_inc_error_counter(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_DATA_NACK:
            chsm_exit_children(self, e_pst, ctx_pst);
            vcnl4040_inc_error_counter(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_SUCCESS:
            chsm_exit_children(self, e_pst, ctx_pst);
            vcnl4040_update_data(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        default:
            guards_only_b = false;
    }

    if(vcnl4040_timeout(self, e_pst, VCNL4040_READ_PERIOD))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        vcnl4040_inc_error_counter(self, e_pst);
        vcnl4040_reset_timer(self, e_pst);
        vcnl4040_reset_timer(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_online, vcnl4040_reset_timer, guards_only_b);
}

chsm_result_ten vcnl4040_top(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            chsm_exit_children(self, e_pst, ctx_pst);
            vcnl4040_init(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_read_id(self, e_pst);
            return chsm_transition(self, s_read_id);

        default:
            guards_only_b = false;
    }

    return chsm_ignored(self);
}
