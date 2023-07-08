/*Generated with CHSM v0.0.0 at 2023.07.02 22.34.14*/
#include "cevent.h"
#include "chsm.h"
#include "vcnl4040.h"
#include "vcnl4040_functions.h"


static chsm_result_ten s_reading(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_unplugged(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_read_id(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_config(chsm_tst *self, const cevent_tst  *e_pst);
char vcnl4040_debug_state_ac[20];

static chsm_result_ten s_config(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_10ms:
            vcnl4040_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
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
            vcnl4040_debug_log_func(self, e_pst, "SIG_I2C_RESULT_ADDR_NACK", __FUNCTION__);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_DATA_NACK:
            vcnl4040_debug_log_func(self, e_pst, "SIG_I2C_RESULT_DATA_NACK", __FUNCTION__);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_unplugged);

        case SIG_I2C_RESULT_SUCCESS:
            vcnl4040_debug_log_func(self, e_pst, "SIG_I2C_RESULT_SUCCESS", __FUNCTION__);
            send_online_event(self, e_pst);
            vcnl4040_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        case SIG_SYS_TICK_10ms:
            vcnl4040_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
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
            vcnl4040_debug_log_func(self, e_pst, "SIG_I2C_RESULT_SUCCESS", __FUNCTION__);
            send_online_event(self, e_pst);
            vcnl4040_start_read(self, e_pst);
            return chsm_transition(self, s_reading);

        case SIG_SYS_TICK_10ms:
            vcnl4040_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
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
        case SIG_SYS_TICK_10ms:
            vcnl4040_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
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
            vcnl4040_debug_log_func(self, e_pst, "SIG_I2C_RESULT_ADDR_NACK", __FUNCTION__);
            vcnl4040_inc_error_counter(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_DATA_NACK:
            vcnl4040_debug_log_func(self, e_pst, "SIG_I2C_RESULT_DATA_NACK", __FUNCTION__);
            vcnl4040_inc_error_counter(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_RESULT_SUCCESS:
            vcnl4040_debug_log_func(self, e_pst, "SIG_I2C_RESULT_SUCCESS", __FUNCTION__);
            vcnl4040_update_data(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_10ms:
            vcnl4040_debug_log_func(self, e_pst, "SIG_SYS_TICK_10ms", __FUNCTION__);
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

chsm_result_ten vcnl4040_top(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            vcnl4040_debug_log_func(self, e_pst, "C_SIG_INIT", __FUNCTION__);
            vcnl4040_init(self, e_pst);
            vcnl4040_reset_timer(self, e_pst);
            vcnl4040_read_id(self, e_pst);
            return chsm_transition(self, s_read_id);
    }

    return chsm_ignored(self);
}

void vcnl4040_debug_log_func(chsm_tst *self, const cevent_tst *est, uint8_t *trans_name, const char *state_func) 
{
	#ifdef CHSM_BUILD_TESTS 
		printf("vcnl4040_%s --%s-->\n", state_func, trans_name); 
	#else 
		CRF_UNUSED(self); 
		CRF_UNUSED(est); 
		CRF_UNUSED(trans_name); 
		CRF_UNUSED(state_func); 
		memcpy(vcnl4040_debug_state_ac, 0, 20); 
		memcpy(vcnl4040_debug_state_ac, state_func, 20); 
	#endif 
}  
