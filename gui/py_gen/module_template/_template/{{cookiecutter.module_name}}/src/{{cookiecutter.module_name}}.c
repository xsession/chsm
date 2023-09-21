/*Generated with CHSM v0.0.0 at 2023.07.31 10.25.58*/
#include "cevent.h"
#include "chsm.h"
#include <string.h>
#include "{{cookiecutter.module_name}}.h"
#include "{{cookiecutter.module_name}}_functions.h"


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
char {{cookiecutter.module_name}}_debug_state_ac[20];

static chsm_result_ten s_read_id_reg(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_ADDR_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_unplugged(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_DATA_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_DATA_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_unplugged(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_SUCCESS:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_SUCCESS", __FUNCTION__);
            if({{cookiecutter.module_name}}_id_match(self, e_pst))
            {
                {{cookiecutter.module_name}}_reset_timer(self, e_pst);
                {{cookiecutter.module_name}}_reset_timer(self, e_pst);
                {{cookiecutter.module_name}}_set_full_powerdown(self, e_pst);
                return chsm_transition(self, s_power_down);
            }
            break;

        case SIG_SYS_TICK_10ms:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
            {{cookiecutter.module_name}}_10ms_callback(self, e_pst);
            break;
    }

    if({{cookiecutter.module_name}}_timeout(self, e_pst, {{cookiecutter.module_name|upper}}_RETRY_TIMEOUT))
    {
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_unplugged(self, e_pst);
        return chsm_transition(self, s_unplugged);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
            {{cookiecutter.module_name}}_10ms_callback(self, e_pst);
            break;
    }

    if({{cookiecutter.module_name}}_timeout(self, e_pst, {{cookiecutter.module_name|upper}}_UNPLUGGED_TIMEOUT))
    {
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_set_resolution(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_ADDR_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_set_resolution(self, e_pst);
            return chsm_transition(self, s_set_resolution);

        case SIG_I2C_RESULT_DATA_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_DATA_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_set_resolution(self, e_pst);
            return chsm_transition(self, s_set_resolution);

        case SIG_I2C_RESULT_SUCCESS:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_SUCCESS", __FUNCTION__);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_set_full_powerup(self, e_pst);
            return chsm_transition(self, s_power_up);

        case SIG_SYS_TICK_10ms:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
            {{cookiecutter.module_name}}_10ms_callback(self, e_pst);
            break;
    }

    if({{cookiecutter.module_name}}_timeout(self, e_pst, {{cookiecutter.module_name|upper}}_RETRY_TIMEOUT))
    {
        {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_set_resolution(self, e_pst);
        return chsm_transition(self, s_set_resolution);
    }

    if({{cookiecutter.module_name}}_error_count(self, e_pst, {{cookiecutter.module_name|upper}}_MAX_ERROR_COUNT))
    {
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_reset_error_cnt(self, e_pst);
        {{cookiecutter.module_name}}_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_reset_ptr_reg(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_SUCCESS:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_SUCCESS", __FUNCTION__);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            send_online_event(self, e_pst);
            {{cookiecutter.module_name}}_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        case SIG_I2C_RESULT_ADDR_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_ADDR_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_reset_pointer(self, e_pst);
            return chsm_transition(self, s_reset_ptr_reg);

        case SIG_I2C_RESULT_DATA_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_DATA_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_reset_pointer(self, e_pst);
            return chsm_transition(self, s_reset_ptr_reg);

        case SIG_SYS_TICK_10ms:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
            {{cookiecutter.module_name}}_10ms_callback(self, e_pst);
            break;
    }

    if({{cookiecutter.module_name}}_timeout(self, e_pst, {{cookiecutter.module_name|upper}}_RETRY_TIMEOUT))
    {
        {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_reset_pointer(self, e_pst);
        return chsm_transition(self, s_reset_ptr_reg);
    }

    if({{cookiecutter.module_name}}_error_count(self, e_pst, {{cookiecutter.module_name|upper}}_MAX_ERROR_COUNT))
    {
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_reset_error_cnt(self, e_pst);
        {{cookiecutter.module_name}}_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_{{cookiecutter.module_name|upper}}_READ:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_{{cookiecutter.module_name|upper}}_READ", __FUNCTION__);
            {{cookiecutter.module_name}}_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        case SIG_SYS_TICK_10ms:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
            {{cookiecutter.module_name}}_10ms_callback(self, e_pst);
            break;
    }

    if({{cookiecutter.module_name}}_timeout(self, e_pst, {{cookiecutter.module_name|upper}}_READ_PERIOD))
    {
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_start_read(self, e_pst);
        return chsm_transition(self, s_reading);
    }

    if({{cookiecutter.module_name}}_error_count(self, e_pst, {{cookiecutter.module_name|upper}}_MAX_ERROR_COUNT))
    {
        send_offline_event(self, e_pst);
        {{cookiecutter.module_name}}_reset_error_cnt(self, e_pst);
        {{cookiecutter.module_name}}_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_reading(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_ADDR_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_DATA_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_DATA_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_SUCCESS:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_SUCCESS", __FUNCTION__);
            {{cookiecutter.module_name}}_update_temp(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_10ms:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
            {{cookiecutter.module_name}}_10ms_callback(self, e_pst);
            break;
    }

    if({{cookiecutter.module_name}}_timeout(self, e_pst, {{cookiecutter.module_name|upper}}_READ_PERIOD))
    {
        {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    if({{cookiecutter.module_name}}_error_count(self, e_pst, {{cookiecutter.module_name|upper}}_MAX_ERROR_COUNT))
    {
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        send_offline_event(self, e_pst);
        {{cookiecutter.module_name}}_reset_error_cnt(self, e_pst);
        {{cookiecutter.module_name}}_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_power_down(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_ADDR_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_set_full_powerdown(self, e_pst);
            return chsm_transition(self, s_power_down);

        case SIG_I2C_RESULT_DATA_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_DATA_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_set_full_powerdown(self, e_pst);
            return chsm_transition(self, s_power_down);

        case SIG_I2C_RESULT_SUCCESS:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_SUCCESS", __FUNCTION__);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_init_wait(self, e_pst);
            return chsm_transition(self, s_wait_power_down);

        case SIG_SYS_TICK_10ms:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
            {{cookiecutter.module_name}}_10ms_callback(self, e_pst);
            break;
    }

    if({{cookiecutter.module_name}}_timeout(self, e_pst, {{cookiecutter.module_name|upper}}_RETRY_TIMEOUT))
    {
        {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_set_full_powerdown(self, e_pst);
        return chsm_transition(self, s_power_down);
    }

    if({{cookiecutter.module_name}}_error_count(self, e_pst, {{cookiecutter.module_name|upper}}_MAX_ERROR_COUNT))
    {
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_reset_error_cnt(self, e_pst);
        {{cookiecutter.module_name}}_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_power_up(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_RESULT_ADDR_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_ADDR_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_set_full_powerup(self, e_pst);
            return chsm_transition(self, s_power_up);

        case SIG_I2C_RESULT_DATA_NACK:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_DATA_NACK", __FUNCTION__);
            {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_set_full_powerup(self, e_pst);
            return chsm_transition(self, s_power_up);

        case SIG_I2C_RESULT_SUCCESS:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_I2C_RESULT_SUCCESS", __FUNCTION__);
            {{cookiecutter.module_name}}_reset_timer(self, e_pst);
            {{cookiecutter.module_name}}_init_wait(self, e_pst);
            return chsm_transition(self, s_wait_power_up);

        case SIG_SYS_TICK_10ms:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
            {{cookiecutter.module_name}}_10ms_callback(self, e_pst);
            break;
    }

    if({{cookiecutter.module_name}}_timeout(self, e_pst, {{cookiecutter.module_name|upper}}_RETRY_TIMEOUT))
    {
        {{cookiecutter.module_name}}_inc_error_counter(self, e_pst);
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_set_full_powerup(self, e_pst);
        return chsm_transition(self, s_power_up);
    }

    if({{cookiecutter.module_name}}_error_count(self, e_pst, {{cookiecutter.module_name|upper}}_MAX_ERROR_COUNT))
    {
        {{cookiecutter.module_name}}_reset_timer(self, e_pst);
        {{cookiecutter.module_name}}_reset_error_cnt(self, e_pst);
        {{cookiecutter.module_name}}_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_wait_power_down(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
            {{cookiecutter.module_name}}_10ms_callback(self, e_pst);
            break;
    }

    if({{cookiecutter.module_name}}_wait_cnt(self, e_pst))
    {
        {{cookiecutter.module_name}}_set_resolution(self, e_pst);
        return chsm_transition(self, s_set_resolution);
    }

    if({{cookiecutter.module_name}}_error_count(self, e_pst, {{cookiecutter.module_name|upper}}_MAX_ERROR_COUNT))
    {
        {{cookiecutter.module_name}}_reset_error_cnt(self, e_pst);
        {{cookiecutter.module_name}}_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_wait_power_up(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
            {{cookiecutter.module_name}}_10ms_callback(self, e_pst);
            break;
    }

    if({{cookiecutter.module_name}}_wait_cnt(self, e_pst))
    {
        {{cookiecutter.module_name}}_reset_pointer(self, e_pst);
        return chsm_transition(self, s_reset_ptr_reg);
    }

    if({{cookiecutter.module_name}}_error_count(self, e_pst, {{cookiecutter.module_name|upper}}_MAX_ERROR_COUNT))
    {
        {{cookiecutter.module_name}}_reset_error_cnt(self, e_pst);
        {{cookiecutter.module_name}}_read_id(self, e_pst);
        return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

chsm_result_ten {{cookiecutter.module_name}}_top(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            {{cookiecutter.module_name}}_debug_log_func(self, e_pst, "C_SIG_INIT", __FUNCTION__);
            {{cookiecutter.module_name}}_init(self, e_pst);
            {{cookiecutter.module_name}}_reset_error_cnt(self, e_pst);
            {{cookiecutter.module_name}}_read_id(self, e_pst);
            return chsm_transition(self, s_read_id_reg);
    }

    return chsm_ignored(self);
}

void {{cookiecutter.module_name}}_debug_log_func(chsm_tst *self, const cevent_tst *est, uint8_t *trans_name, const char *state_func) 
{
	#ifdef CHSM_BUILD_TESTS 
		printf("{{cookiecutter.module_name}}_%s --%s-->\n", state_func, trans_name); 
	#else 
		CRF_UNUSED(self); 
		CRF_UNUSED(est); 
		CRF_UNUSED(trans_name); 
		CRF_UNUSED(state_func); 
		memset({{cookiecutter.module_name}}_debug_state_ac, 0, 20); 
		strncpy({{cookiecutter.module_name}}_debug_state_ac, state_func, 19); 
		{{cookiecutter.module_name}}_debug_state_ac[19] = '\0'; 
	#endif 
}  
