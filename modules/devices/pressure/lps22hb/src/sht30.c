/*Generated with CHSM v0.0.0 at 2021.11.25 16.00.24*/
#include "sht30.h"
#include "cevent.h"
#include "chsm.h"
#include "sht30_functions.h"

static chsm_result_ten s_clear_status_reg(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_measurement_init_read(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_measurement_init_write(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_init(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_init_read(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_check_status(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_online(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_measurement_write(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s_sht30(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst);

static chsm_result_ten s_sht30(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            sht30_10ms_callback(self, e_pst);
            break;

        case SIG_SHT30_I2C_POWER_STATE:
            sht30_i2c_power_state(self, e_pst);
            break;

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, sht30_top, NULL, guards_only_b);
}

static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {

        default:
            guards_only_b = false;
    }

    if (sht30_timeout(self, e_pst, SHT30_READ_PERIOD))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        sht30_reset_timer(self, e_pst);
        sht30_measurement_reg_write(self, e_pst);
        return chsm_transition(self, s_measurement_write);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_online, NULL, guards_only_b);
}

static chsm_result_ten s_measurement_write(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            chsm_exit_children(self, e_pst, ctx_pst);
            sht30_inc_error_counter(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_DATA_NACK:
            chsm_exit_children(self, e_pst, ctx_pst);
            sht30_inc_error_counter(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_SUCCESS:
            chsm_exit_children(self, e_pst, ctx_pst);
            sht30_update_data(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        default:
            guards_only_b = false;
    }

    if (sht30_timeout(self, e_pst, SHT30_READ_PERIOD))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        sht30_reset_timer(self, e_pst);
        sht30_reset_timer(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_online, NULL, guards_only_b);
}

static chsm_result_ten s_online(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {

        default:
            guards_only_b = false;
    }

    if (sht30_error_count(self, e_pst, SHT30_MAX_ERROR_COUNT))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        sht30_send_offline_event(self, e_pst);
        sht30_init(self, e_pst);
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_sht30, sht30_send_offline_event, guards_only_b);
}

static chsm_result_ten s_check_status(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            chsm_exit_children(self, e_pst, ctx_pst);
            sht30_check_status(self, e_pst);
            sht30_reset_timer(self, e_pst);
            sht30_clear_status_reg(self, e_pst);
            return chsm_transition(self, s_clear_status_reg);

        case SIG_I2C_RESULT_ADDR_NACK:
            chsm_exit_children(self, e_pst, ctx_pst);
            sht30_unplugged(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_DATA_NACK:
            chsm_exit_children(self, e_pst, ctx_pst);
            sht30_unplugged(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        default:
            guards_only_b = false;
    }

    chsm_exit_children(self, e_pst, ctx_pst);
    sht30_unplugged(self, e_pst);
    sht30_reset_timer(self, e_pst);
    return chsm_transition(self, s_unplugged);

    return chsm_handle_in_parent(self, ctx_pst, s_init, NULL, guards_only_b);
}

static chsm_result_ten s_init_read(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {

        default:
            guards_only_b = false;
    }

    if (sht30_timeout(self, e_pst, SHT30_UNPLUG_RETRY))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_init, NULL, guards_only_b);
}

static chsm_result_ten s_init(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, s_sht30, NULL, guards_only_b);
}

static chsm_result_ten s_measurement_init_write(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {

        default:
            guards_only_b = false;
    }

    if (sht30_timeout(self, e_pst, SHT30_CONVERSION_TIME))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        sht30_reset_timer(self, e_pst);
        sht30_measurement_reg_read(self, e_pst);
        return chsm_transition(self, s_measurement_init_read);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_init_read, sht30_reset_timer, guards_only_b);
}

static chsm_result_ten s_measurement_init_read(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            chsm_exit_children(self, e_pst, ctx_pst);
            sht30_send_online_event(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, s_init_read, NULL, guards_only_b);
}

static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {

        default:
            guards_only_b = false;
    }

    if (sht30_timeout(self, e_pst, SHT30_UNPLUG_RETRY))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    return chsm_handle_in_parent(self, ctx_pst, s_init, NULL, guards_only_b);
}

static chsm_result_ten s_clear_status_reg(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            chsm_exit_children(self, e_pst, ctx_pst);
            sht30_init_measurement_reg_write(self, e_pst);
            return chsm_transition(self, s_measurement_init_write);

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, s_init_read, NULL, guards_only_b);
}

chsm_result_ten sht30_top(chsm_tst *self, const cevent_tst *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b = true;
    switch (e_pst->sig)
    {
        case C_SIG_INIT:
            chsm_exit_children(self, e_pst, ctx_pst);
            sht30_init(self, e_pst);
            sht30_init(self, e_pst);
            sht30_get_status(self, e_pst);
            return chsm_transition(self, s_check_status);

        default:
            guards_only_b = false;
    }

    return chsm_ignored(self);
}
