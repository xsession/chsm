/*Generated with CHSM v0.0.0 at 2023.05.17 12.11.21*/
#include "cevent.h"
#include "chsm.h"
#include "sht30.h"
#include "sht30_functions.h"


static chsm_result_ten s_measurement_write(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_clear_status_reg(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_measurement_init_read(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_measurement_init_write(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_check_status(chsm_tst *self, const cevent_tst  *e_pst);

static chsm_result_ten s_check_status(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            sht30_check_status(self, e_pst);
            sht30_reset_timer(self, e_pst);
            sht30_clear_status_reg(self, e_pst);
            return chsm_transition(self, s_clear_status_reg);

        case SIG_I2C_RESULT_ADDR_NACK:
            sht30_unplugged(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_DATA_NACK:
            sht30_unplugged(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_SYS_TICK_10ms:
            sht30_10ms_callback(self, e_pst);
            break;

        case SIG_SHT30_I2C_POWER_STATE:
            sht30_i2c_power_state(self, e_pst);
            break;
    }

    sht30_init(self, e_pst);
    sht30_init(self, e_pst);
    sht30_unplugged(self, e_pst);
    sht30_reset_timer(self, e_pst);
    return chsm_transition(self, s_unplugged);

    return chsm_ignored(self);
}

static chsm_result_ten s_measurement_init_write(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            sht30_inc_error_counter(self, e_pst);
            sht30_reset_timer(self, e_pst);
            sht30_init_measurement_reg_write(self, e_pst);
            return chsm_transition(self, s_measurement_init_write);

        case SIG_I2C_RESULT_DATA_NACK:
            sht30_inc_error_counter(self, e_pst);
            sht30_reset_timer(self, e_pst);
            sht30_init_measurement_reg_write(self, e_pst);
            return chsm_transition(self, s_measurement_init_write);

        case SIG_SYS_TICK_10ms:
            sht30_10ms_callback(self, e_pst);
            break;

        case SIG_SHT30_I2C_POWER_STATE:
            sht30_i2c_power_state(self, e_pst);
            break;
    }

    if(sht30_timeout(self, e_pst, SHT30_CONVERSION_TIME))
    {
        sht30_reset_timer(self, e_pst);
        sht30_measurement_reg_read(self, e_pst);
        return chsm_transition(self, s_measurement_init_read);
    }

    if(sht30_timeout(self, e_pst, SHT30_UNPLUG_RETRY))
    {
        sht30_reset_timer(self, e_pst);
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    if(sht30_error_count(self, e_pst, SHT30_MAX_ERROR_COUNT))
    {
        sht30_reset_timer(self, e_pst);
        sht30_init(self, e_pst);
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_measurement_init_read(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            sht30_send_online_event(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_ADDR_NACK:
            sht30_inc_error_counter(self, e_pst);
            sht30_measurement_reg_read(self, e_pst);
            return chsm_transition(self, s_measurement_init_read);

        case SIG_I2C_RESULT_DATA_NACK:
            sht30_inc_error_counter(self, e_pst);
            sht30_measurement_reg_read(self, e_pst);
            return chsm_transition(self, s_measurement_init_read);

        case SIG_SYS_TICK_10ms:
            sht30_10ms_callback(self, e_pst);
            break;

        case SIG_SHT30_I2C_POWER_STATE:
            sht30_i2c_power_state(self, e_pst);
            break;
    }

    if(sht30_timeout(self, e_pst, SHT30_UNPLUG_RETRY))
    {
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    if(sht30_error_count(self, e_pst, SHT30_MAX_ERROR_COUNT))
    {
        sht30_init(self, e_pst);
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            sht30_10ms_callback(self, e_pst);
            break;

        case SIG_SHT30_I2C_POWER_STATE:
            sht30_i2c_power_state(self, e_pst);
            break;
    }

    if(sht30_timeout(self, e_pst, SHT30_UNPLUG_RETRY))
    {
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_clear_status_reg(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            sht30_init_measurement_reg_write(self, e_pst);
            return chsm_transition(self, s_measurement_init_write);

        case SIG_I2C_RESULT_ADDR_NACK:
            sht30_inc_error_counter(self, e_pst);
            sht30_clear_status_reg(self, e_pst);
            return chsm_transition(self, s_clear_status_reg);

        case SIG_I2C_RESULT_DATA_NACK:
            sht30_inc_error_counter(self, e_pst);
            sht30_clear_status_reg(self, e_pst);
            return chsm_transition(self, s_clear_status_reg);

        case SIG_SYS_TICK_10ms:
            sht30_10ms_callback(self, e_pst);
            break;

        case SIG_SHT30_I2C_POWER_STATE:
            sht30_i2c_power_state(self, e_pst);
            break;
    }

    if(sht30_timeout(self, e_pst, SHT30_UNPLUG_RETRY))
    {
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    if(sht30_error_count(self, e_pst, SHT30_MAX_ERROR_COUNT))
    {
        sht30_init(self, e_pst);
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            sht30_10ms_callback(self, e_pst);
            break;

        case SIG_SHT30_I2C_POWER_STATE:
            sht30_i2c_power_state(self, e_pst);
            break;
    }

    if(sht30_timeout(self, e_pst, SHT30_READ_PERIOD))
    {
        sht30_reset_timer(self, e_pst);
        sht30_measurement_reg_write(self, e_pst);
        return chsm_transition(self, s_measurement_write);
    }

    if(sht30_error_count(self, e_pst, SHT30_MAX_ERROR_COUNT))
    {
        sht30_send_offline_event(self, e_pst);
        sht30_init(self, e_pst);
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_measurement_write(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            sht30_inc_error_counter(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_DATA_NACK:
            sht30_inc_error_counter(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_SUCCESS:
            sht30_update_data(self, e_pst);
            sht30_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_10ms:
            sht30_10ms_callback(self, e_pst);
            break;

        case SIG_SHT30_I2C_POWER_STATE:
            sht30_i2c_power_state(self, e_pst);
            break;
    }

    if(sht30_timeout(self, e_pst, SHT30_READ_PERIOD))
    {
        sht30_reset_timer(self, e_pst);
        sht30_reset_timer(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    if(sht30_error_count(self, e_pst, SHT30_MAX_ERROR_COUNT))
    {
        sht30_send_offline_event(self, e_pst);
        sht30_init(self, e_pst);
        sht30_get_status(self, e_pst);
        return chsm_transition(self, s_check_status);
    }

    return chsm_ignored(self);
}

chsm_result_ten sht30_top(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            sht30_init(self, e_pst);
            sht30_init(self, e_pst);
            sht30_get_status(self, e_pst);
            return chsm_transition(self, s_check_status);
    }

    return chsm_ignored(self);
}
