/*Generated with CHSM v0.0.0 at 2023.05.11 15.32.34*/
#include "cevent.h"
#include "chsm.h"
#include "tmp468.h"
#include "tmp468_functions.h"


static chsm_result_ten s_unlock_regs(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_set_n_factor4(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_set_n_factor3(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_set_n_factor2(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_set_n_factor1(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_read_sensor_status(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_read_id_reg(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_read_temp(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst);

static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            tmp468_10ms_callback(self, e_pst);
            break;

        case SIG_TMP468_I2C_POWER_STATE:
            tmp468_i2c_power_state(self, e_pst);
            break;
    }

    if(tmp468_status_timeout(self, e_pst, TMP468_STATUS_READ_PERIOD))
    {
        tmp468_reset_status_timer(self, e_pst);
        tmp468_read_sensor_status(self, e_pst);
        return chsm_transition(self, s_read_sensor_status);
    }

    if(tmp468_timeout(self, e_pst, TMP468_READ_PERIOD))
    {
        tmp468_reset_timer(self, e_pst);
        tmp468_read_temp(self, e_pst);
        return chsm_transition(self, s_read_temp);
    }

    if(tmp468_error_count(self, e_pst, TMP468_MAX_ERROR_COUNT))
    {
        tmp468_send_offline_event(self, e_pst);
        tmp468_reset_timer(self, e_pst);
        tmp468_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_read_temp(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            tmp468_inc_error_counter(self, e_pst);
            tmp468_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_SUCCESS:
            tmp468_update_temp(self, e_pst);
            tmp468_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_DATA_NACK:
            tmp468_inc_error_counter(self, e_pst);
            tmp468_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_10ms:
            tmp468_10ms_callback(self, e_pst);
            break;

        case SIG_TMP468_I2C_POWER_STATE:
            tmp468_i2c_power_state(self, e_pst);
            break;
    }

    if(tmp468_timeout(self, e_pst, TMP468_READ_PERIOD))
    {
        tmp468_inc_error_counter(self, e_pst);
        tmp468_reset_timer(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    if(tmp468_error_count(self, e_pst, TMP468_MAX_ERROR_COUNT))
    {
        tmp468_send_offline_event(self, e_pst);
        tmp468_reset_timer(self, e_pst);
        tmp468_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_read_id_reg(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            if(tmp468_id_match(self, e_pst))
            {
                tmp468_reset_timer(self, e_pst);
                tmp468_unlock_regs(self, e_pst);
                return chsm_transition(self, s_unlock_regs);
            }
            break;

        case SIG_I2C_RESULT_ADDR_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_read_id(self, e_pst);
            return chsm_transition(self, s_read_id_reg);

        case SIG_I2C_RESULT_DATA_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_read_id(self, e_pst);
            return chsm_transition(self, s_read_id_reg);

        case SIG_SYS_TICK_10ms:
            tmp468_10ms_callback(self, e_pst);
            break;

        case SIG_TMP468_I2C_POWER_STATE:
            tmp468_i2c_power_state(self, e_pst);
            break;
    }

    if(tmp468_timeout(self, e_pst, TMP468_RETRY_TIMEOUT))
    {
        tmp468_reset_timer(self, e_pst);
        tmp468_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_read_sensor_status(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            tmp468_inc_error_counter(self, e_pst);
            tmp468_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_DATA_NACK:
            tmp468_inc_error_counter(self, e_pst);
            tmp468_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_SUCCESS:
            tmp468_update_status(self, e_pst);
            tmp468_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_10ms:
            tmp468_10ms_callback(self, e_pst);
            break;

        case SIG_TMP468_I2C_POWER_STATE:
            tmp468_i2c_power_state(self, e_pst);
            break;
    }

    if(tmp468_status_timeout(self, e_pst, TMP468_STATUS_READ_PERIOD))
    {
        tmp468_inc_error_counter(self, e_pst);
        tmp468_reset_timer(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    if(tmp468_error_count(self, e_pst, TMP468_MAX_ERROR_COUNT))
    {
        tmp468_send_offline_event(self, e_pst);
        tmp468_reset_timer(self, e_pst);
        tmp468_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_set_n_factor1(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor2(self, e_pst);
            return chsm_transition(self, s_set_n_factor2);

        case SIG_I2C_RESULT_ADDR_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor1(self, e_pst);
            return chsm_transition(self, s_set_n_factor1);

        case SIG_I2C_RESULT_DATA_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor1(self, e_pst);
            return chsm_transition(self, s_set_n_factor1);

        case SIG_SYS_TICK_10ms:
            tmp468_10ms_callback(self, e_pst);
            break;

        case SIG_TMP468_I2C_POWER_STATE:
            tmp468_i2c_power_state(self, e_pst);
            break;
    }

    if(tmp468_timeout(self, e_pst, TMP468_RETRY_TIMEOUT))
    {
        tmp468_reset_timer(self, e_pst);
        tmp468_set_n_factor1(self, e_pst);
        return chsm_transition(self, s_set_n_factor1);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_set_n_factor2(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor3(self, e_pst);
            return chsm_transition(self, s_set_n_factor3);

        case SIG_I2C_RESULT_ADDR_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor2(self, e_pst);
            return chsm_transition(self, s_set_n_factor2);

        case SIG_I2C_RESULT_DATA_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor2(self, e_pst);
            return chsm_transition(self, s_set_n_factor2);

        case SIG_SYS_TICK_10ms:
            tmp468_10ms_callback(self, e_pst);
            break;

        case SIG_TMP468_I2C_POWER_STATE:
            tmp468_i2c_power_state(self, e_pst);
            break;
    }

    if(tmp468_timeout(self, e_pst, TMP468_RETRY_TIMEOUT))
    {
        tmp468_reset_timer(self, e_pst);
        tmp468_set_n_factor2(self, e_pst);
        return chsm_transition(self, s_set_n_factor2);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_set_n_factor3(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor4(self, e_pst);
            return chsm_transition(self, s_set_n_factor4);

        case SIG_I2C_RESULT_ADDR_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor3(self, e_pst);
            return chsm_transition(self, s_set_n_factor3);

        case SIG_I2C_RESULT_DATA_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor3(self, e_pst);
            return chsm_transition(self, s_set_n_factor3);

        case SIG_SYS_TICK_10ms:
            tmp468_10ms_callback(self, e_pst);
            break;

        case SIG_TMP468_I2C_POWER_STATE:
            tmp468_i2c_power_state(self, e_pst);
            break;
    }

    if(tmp468_timeout(self, e_pst, TMP468_RETRY_TIMEOUT))
    {
        tmp468_reset_timer(self, e_pst);
        tmp468_set_n_factor3(self, e_pst);
        return chsm_transition(self, s_set_n_factor3);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_set_n_factor4(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            tmp468_send_online_event(self, e_pst);
            tmp468_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_ADDR_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor4(self, e_pst);
            return chsm_transition(self, s_set_n_factor4);

        case SIG_I2C_RESULT_DATA_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor4(self, e_pst);
            return chsm_transition(self, s_set_n_factor4);

        case SIG_SYS_TICK_10ms:
            tmp468_10ms_callback(self, e_pst);
            break;

        case SIG_TMP468_I2C_POWER_STATE:
            tmp468_i2c_power_state(self, e_pst);
            break;
    }

    if(tmp468_timeout(self, e_pst, TMP468_RETRY_TIMEOUT))
    {
        tmp468_reset_timer(self, e_pst);
        tmp468_set_n_factor4(self, e_pst);
        return chsm_transition(self, s_set_n_factor4);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_unlock_regs(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            tmp468_reset_timer(self, e_pst);
            tmp468_set_n_factor1(self, e_pst);
            return chsm_transition(self, s_set_n_factor1);

        case SIG_I2C_RESULT_ADDR_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_unlock_regs(self, e_pst);
            return chsm_transition(self, s_unlock_regs);

        case SIG_I2C_RESULT_DATA_NACK:
            tmp468_reset_timer(self, e_pst);
            tmp468_unlock_regs(self, e_pst);
            return chsm_transition(self, s_unlock_regs);

        case SIG_SYS_TICK_10ms:
            tmp468_10ms_callback(self, e_pst);
            break;

        case SIG_TMP468_I2C_POWER_STATE:
            tmp468_i2c_power_state(self, e_pst);
            break;
    }

    if(tmp468_timeout(self, e_pst, TMP468_RETRY_TIMEOUT))
    {
        tmp468_reset_timer(self, e_pst);
        tmp468_unlock_regs(self, e_pst);
        return chsm_transition(self, s_unlock_regs);
    }

    return chsm_ignored(self);
}

chsm_result_ten tmp468_top(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            tmp468_init(self, e_pst);
            tmp468_reset_timer(self, e_pst);
            tmp468_read_id(self, e_pst);
            return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}
