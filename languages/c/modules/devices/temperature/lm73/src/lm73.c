/*Generated with CHSM v0.0.0 at 2023.05.17 12.59.44*/
#include "cevent.h"
#include "chsm.h"
#include "lm73.h"
#include "lm73_functions.h"


static chsm_result_ten s_wait_power_up(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_wait_power_down(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_power_up(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_power_down(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_reading(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_reset_ptr_reg(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_set_resolution(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_read_id_reg(chsm_tst *self, const cevent_tst  *e_pst);

static chsm_result_ten s_read_id_reg(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            lm73_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_DATA_NACK:
            lm73_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_SUCCESS:
            if(lm73_id_match(self, e_pst))
            {
                lm73_reset_timer(self, e_pst);
                lm73_set_full_powerdown(self, e_pst);
                return chsm_transition(self, s_power_down);
            }
            break;

        case SIG_SYS_TICK_10ms:
            lm73_10ms_callback(self, e_pst);
            break;
    }

    if(lm73_timeout(self, e_pst, LM73_RETRY_TIMEOUT))
    {
        lm73_reset_timer(self, e_pst);
        return chsm_transition(self, s_unplugged);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            lm73_10ms_callback(self, e_pst);
            break;
    }

    if(lm73_timeout(self, e_pst, LM73_UNPLUGGED_TIMEOUT))
    {
        lm73_reset_timer(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_set_resolution(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            lm73_inc_error_counter(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_set_resolution(self, e_pst);
            return chsm_transition(self, s_set_resolution);

        case SIG_I2C_RESULT_DATA_NACK:
            lm73_inc_error_counter(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_set_resolution(self, e_pst);
            return chsm_transition(self, s_set_resolution);

        case SIG_I2C_RESULT_SUCCESS:
            lm73_reset_timer(self, e_pst);
            lm73_set_full_powerup(self, e_pst);
            return chsm_transition(self, s_power_up);

        case SIG_SYS_TICK_10ms:
            lm73_10ms_callback(self, e_pst);
            break;
    }

    if(lm73_timeout(self, e_pst, LM73_RETRY_TIMEOUT))
    {
        lm73_inc_error_counter(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_set_resolution(self, e_pst);
        return chsm_transition(self, s_set_resolution);
    }

    if(lm73_error_count(self, e_pst, LM73_MAX_ERROR_COUNT))
    {
        lm73_reset_timer(self, e_pst);
        lm73_reset_error_cnt(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_reset_ptr_reg(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            lm73_reset_timer(self, e_pst);
            send_online_event(self, e_pst);
            lm73_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        case SIG_I2C_RESULT_ADDR_NACK:
            lm73_inc_error_counter(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_reset_pointer(self, e_pst);
            return chsm_transition(self, s_reset_ptr_reg);

        case SIG_I2C_RESULT_DATA_NACK:
            lm73_inc_error_counter(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_reset_pointer(self, e_pst);
            return chsm_transition(self, s_reset_ptr_reg);

        case SIG_SYS_TICK_10ms:
            lm73_10ms_callback(self, e_pst);
            break;
    }

    if(lm73_timeout(self, e_pst, LM73_RETRY_TIMEOUT))
    {
        lm73_inc_error_counter(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_reset_pointer(self, e_pst);
        return chsm_transition(self, s_reset_ptr_reg);
    }

    if(lm73_error_count(self, e_pst, LM73_MAX_ERROR_COUNT))
    {
        lm73_reset_timer(self, e_pst);
        lm73_reset_error_cnt(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_LM73_READ:
            lm73_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        case SIG_SYS_TICK_10ms:
            lm73_10ms_callback(self, e_pst);
            break;
    }

    if(lm73_timeout(self, e_pst, LM73_READ_PERIOD))
    {
        lm73_reset_timer(self, e_pst);
        lm73_start_read(self, e_pst);
        return chsm_transition(self, s_reading);
    }

    if(lm73_error_count(self, e_pst, LM73_MAX_ERROR_COUNT))
    {
        send_offline_event(self, e_pst);
        lm73_reset_error_cnt(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_reading(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            lm73_inc_error_counter(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_DATA_NACK:
            lm73_inc_error_counter(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_SUCCESS:
            lm73_update_temp(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_10ms:
            lm73_10ms_callback(self, e_pst);
            break;
    }

    if(lm73_timeout(self, e_pst, LM73_READ_PERIOD))
    {
        lm73_inc_error_counter(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_reset_timer(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    if(lm73_error_count(self, e_pst, LM73_MAX_ERROR_COUNT))
    {
        lm73_reset_timer(self, e_pst);
        send_offline_event(self, e_pst);
        lm73_reset_error_cnt(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_power_down(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            lm73_inc_error_counter(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_set_full_powerdown(self, e_pst);
            return chsm_transition(self, s_power_down);

        case SIG_I2C_RESULT_DATA_NACK:
            lm73_inc_error_counter(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_set_full_powerdown(self, e_pst);
            return chsm_transition(self, s_power_down);

        case SIG_I2C_RESULT_SUCCESS:
            lm73_reset_timer(self, e_pst);
            lm73_init_wait(self, e_pst);
            return chsm_transition(self, s_wait_power_down);

        case SIG_SYS_TICK_10ms:
            lm73_10ms_callback(self, e_pst);
            break;
    }

    if(lm73_timeout(self, e_pst, LM73_RETRY_TIMEOUT))
    {
        lm73_inc_error_counter(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_set_full_powerdown(self, e_pst);
        return chsm_transition(self, s_power_down);
    }

    if(lm73_error_count(self, e_pst, LM73_MAX_ERROR_COUNT))
    {
        lm73_reset_timer(self, e_pst);
        lm73_reset_error_cnt(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_power_up(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            lm73_inc_error_counter(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_set_full_powerup(self, e_pst);
            return chsm_transition(self, s_power_up);

        case SIG_I2C_RESULT_DATA_NACK:
            lm73_inc_error_counter(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_set_full_powerup(self, e_pst);
            return chsm_transition(self, s_power_up);

        case SIG_I2C_RESULT_SUCCESS:
            lm73_reset_timer(self, e_pst);
            lm73_init_wait(self, e_pst);
            return chsm_transition(self, s_wait_power_up);

        case SIG_SYS_TICK_10ms:
            lm73_10ms_callback(self, e_pst);
            break;
    }

    if(lm73_timeout(self, e_pst, LM73_RETRY_TIMEOUT))
    {
        lm73_inc_error_counter(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_set_full_powerup(self, e_pst);
        return chsm_transition(self, s_power_up);
    }

    if(lm73_error_count(self, e_pst, LM73_MAX_ERROR_COUNT))
    {
        lm73_reset_timer(self, e_pst);
        lm73_reset_error_cnt(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_wait_power_down(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            lm73_10ms_callback(self, e_pst);
            break;
    }

    if(lm73_wait_cnt(self, e_pst))
    {
        lm73_set_resolution(self, e_pst);
        return chsm_transition(self, s_set_resolution);
    }

    if(lm73_error_count(self, e_pst, LM73_MAX_ERROR_COUNT))
    {
        lm73_reset_error_cnt(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_wait_power_up(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            lm73_10ms_callback(self, e_pst);
            break;
    }

    if(lm73_wait_cnt(self, e_pst))
    {
        lm73_reset_pointer(self, e_pst);
        return chsm_transition(self, s_reset_ptr_reg);
    }

    if(lm73_error_count(self, e_pst, LM73_MAX_ERROR_COUNT))
    {
        lm73_reset_error_cnt(self, e_pst);
        lm73_reset_timer(self, e_pst);
        lm73_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

chsm_result_ten lm73_top(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            lm73_init(self, e_pst);
            lm73_reset_error_cnt(self, e_pst);
            lm73_reset_timer(self, e_pst);
            lm73_read_id(self, e_pst);
            return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}
