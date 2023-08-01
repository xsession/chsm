/*Generated with CHSM v0.0.0 at 2023.08.01 12.01.29*/
#include "cevent.h"
#include "chsm.h"
#include <string.h>
#include "i2c_master.h"
#include "i2c_master_functions.h"


static chsm_result_ten s_reset_periph(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_i2c_master_reset_slave_comm(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_scan_write(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_scan_idle(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_wr_read(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_wr_write(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_read(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_write(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst);
char i2c_master_debug_state_ac[20];

static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_W_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_W_TRANSACTION", __FUNCTION__);
            store_transaction_info(self, e_pst);
            i2c_master_start_tx(self, e_pst);
            return chsm_transition(self, s_write);

        case SIG_I2C_R_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_R_TRANSACTION", __FUNCTION__);
            store_transaction_info(self, e_pst);
            i2c_master_start_rx(self, e_pst);
            return chsm_transition(self, s_read);

        case SIG_I2C_WR_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WR_TRANSACTION", __FUNCTION__);
            store_transaction_info(self, e_pst);
            i2c_master_start_tx(self, e_pst);
            return chsm_transition(self, s_wr_write);

        case SIG_I2C_BUS_SCAN:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_BUS_SCAN", __FUNCTION__);
            scan_init(self, e_pst);
            chsm_recall(self, e_pst);
            return chsm_transition(self, s_scan_idle);

        case SIG_I2C_RESET_SLAVE_COMM:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_RESET_SLAVE_COMM", __FUNCTION__);
            i2c_master_reset_slave_comm(self, e_pst);
            return chsm_transition(self, s_i2c_master_reset_slave_comm);

        case SIG_I2C_RESET_PERIPH:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_RESET_PERIPH", __FUNCTION__);
            i2c_master_reset_periph(self, e_pst);
            return chsm_transition(self, s_reset_periph);

        case SIG_SYS_TICK_1ms:
            i2c_master_debug_log_func(self, e_pst, "SIG_SYS_TICK_1ms", __FUNCTION__);
            i2c_1ms_callback(self, e_pst);
            break;
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_write(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_WRITE_SUCCESS:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WRITE_SUCCESS", __FUNCTION__);
            i2c_master_send_success_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_W_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_W_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_R_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_R_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WR_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WR_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WRITE_FAIL:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WRITE_FAIL", __FUNCTION__);
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_READ_FAIL:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_READ_FAIL", __FUNCTION__);
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_QUEUE_OVERFLOW:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_QUEUE_OVERFLOW", __FUNCTION__);
            i2c_master_clear_queue(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_DEFER_QUEUE_OVERFLOW:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_DEFER_QUEUE_OVERFLOW", __FUNCTION__);
            i2c_master_clear_defer_queue(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_1ms:
            i2c_master_debug_log_func(self, e_pst, "SIG_SYS_TICK_1ms", __FUNCTION__);
            i2c_1ms_callback(self, e_pst);
            break;
    }

    if(i2c_master_fault_cnt(self, e_pst, I2C_MASTER_FAUL_CNT))
    {
        i2c_master_stop(self, e_pst);
        chsm_recall(self, e_pst);
        clear_transaction_info(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_read(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_READ_SUCCESS:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_READ_SUCCESS", __FUNCTION__);
            i2c_master_send_success_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_W_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_W_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_R_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_R_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WR_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WR_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WRITE_FAIL:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WRITE_FAIL", __FUNCTION__);
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_READ_FAIL:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_READ_FAIL", __FUNCTION__);
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_QUEUE_OVERFLOW:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_QUEUE_OVERFLOW", __FUNCTION__);
            i2c_master_clear_queue(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_DEFER_QUEUE_OVERFLOW:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_DEFER_QUEUE_OVERFLOW", __FUNCTION__);
            i2c_master_clear_defer_queue(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_1ms:
            i2c_master_debug_log_func(self, e_pst, "SIG_SYS_TICK_1ms", __FUNCTION__);
            i2c_1ms_callback(self, e_pst);
            break;
    }

    if(i2c_master_fault_cnt(self, e_pst, I2C_MASTER_FAUL_CNT))
    {
        i2c_master_stop(self, e_pst);
        chsm_recall(self, e_pst);
        clear_transaction_info(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_wr_write(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_WRITE_SUCCESS:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WRITE_SUCCESS", __FUNCTION__);
            i2c_master_start_rx(self, e_pst);
            return chsm_transition(self, s_wr_read);

        case SIG_I2C_W_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_W_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_R_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_R_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WR_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WR_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WRITE_FAIL:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WRITE_FAIL", __FUNCTION__);
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_READ_FAIL:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_READ_FAIL", __FUNCTION__);
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_QUEUE_OVERFLOW:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_QUEUE_OVERFLOW", __FUNCTION__);
            i2c_master_clear_queue(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_DEFER_QUEUE_OVERFLOW:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_DEFER_QUEUE_OVERFLOW", __FUNCTION__);
            i2c_master_clear_defer_queue(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_1ms:
            i2c_master_debug_log_func(self, e_pst, "SIG_SYS_TICK_1ms", __FUNCTION__);
            i2c_1ms_callback(self, e_pst);
            break;
    }

    if(i2c_master_fault_cnt(self, e_pst, I2C_MASTER_FAUL_CNT))
    {
        i2c_master_stop(self, e_pst);
        chsm_recall(self, e_pst);
        clear_transaction_info(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_wr_read(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_READ_SUCCESS:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_READ_SUCCESS", __FUNCTION__);
            i2c_master_send_success_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_W_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_W_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_R_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_R_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WR_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WR_TRANSACTION", __FUNCTION__);
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WRITE_FAIL:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WRITE_FAIL", __FUNCTION__);
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_READ_FAIL:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_READ_FAIL", __FUNCTION__);
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_QUEUE_OVERFLOW:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_QUEUE_OVERFLOW", __FUNCTION__);
            i2c_master_clear_queue(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_DEFER_QUEUE_OVERFLOW:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_DEFER_QUEUE_OVERFLOW", __FUNCTION__);
            i2c_master_clear_defer_queue(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_1ms:
            i2c_master_debug_log_func(self, e_pst, "SIG_SYS_TICK_1ms", __FUNCTION__);
            i2c_1ms_callback(self, e_pst);
            break;
    }

    if(i2c_master_fault_cnt(self, e_pst, I2C_MASTER_FAUL_CNT))
    {
        i2c_master_stop(self, e_pst);
        chsm_recall(self, e_pst);
        clear_transaction_info(self, e_pst);
        return chsm_transition(self, s_idle);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_scan_idle(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_W_TRANSACTION:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_W_TRANSACTION", __FUNCTION__);
            store_transaction_info(self, e_pst);
            i2c_scan_start_step(self, e_pst);
            return chsm_transition(self, s_scan_write);

        case SIG_I2C_ADDRS_RELEASE:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_ADDRS_RELEASE", __FUNCTION__);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_1ms:
            i2c_master_debug_log_func(self, e_pst, "SIG_SYS_TICK_1ms", __FUNCTION__);
            i2c_1ms_callback(self, e_pst);
            break;
    }

    if(i2c_master_device_addr_max_cnt(self, e_pst))
    {
        chsm_recall(self, e_pst);
        return chsm_transition(self, s_scan_idle);
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_scan_write(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_WRITE_SUCCESS:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WRITE_SUCCESS", __FUNCTION__);
            i2c_master_inc_success_dev_addr(self, e_pst);
            chsm_recall(self, e_pst);
            return chsm_transition(self, s_scan_idle);

        case SIG_I2C_WRITE_FAIL:
            i2c_master_debug_log_func(self, e_pst, "SIG_I2C_WRITE_FAIL", __FUNCTION__);
            i2c_master_inc_dev_addr(self, e_pst);
            chsm_recall(self, e_pst);
            return chsm_transition(self, s_scan_idle);

        case SIG_SYS_TICK_1ms:
            i2c_master_debug_log_func(self, e_pst, "SIG_SYS_TICK_1ms", __FUNCTION__);
            i2c_1ms_callback(self, e_pst);
            break;
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_i2c_master_reset_slave_comm(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_1ms:
            i2c_master_debug_log_func(self, e_pst, "SIG_SYS_TICK_1ms", __FUNCTION__);
            i2c_1ms_callback(self, e_pst);
            break;
    }

    chsm_recall(self, e_pst);
    clear_transaction_info(self, e_pst);
    return chsm_transition(self, s_idle);

    return chsm_ignored(self);
}

static chsm_result_ten s_reset_periph(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SYS_TICK_1ms:
            i2c_master_debug_log_func(self, e_pst, "SIG_SYS_TICK_1ms", __FUNCTION__);
            i2c_1ms_callback(self, e_pst);
            break;
    }

    chsm_recall(self, e_pst);
    clear_transaction_info(self, e_pst);
    return chsm_transition(self, s_idle);

    return chsm_ignored(self);
}

chsm_result_ten i2c_master_top(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            i2c_master_debug_log_func(self, e_pst, "C_SIG_INIT", __FUNCTION__);
            i2c_master_init(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);
    }

    return chsm_ignored(self);
}

void i2c_master_debug_log_func(chsm_tst *self, const cevent_tst *est, uint8_t *trans_name, const char *state_func) 
{
	#ifdef CHSM_BUILD_TESTS 
		printf("i2c_master_%s --%s-->\n", state_func, trans_name); 
	#else 
		CRF_UNUSED(self); 
		CRF_UNUSED(est); 
		CRF_UNUSED(trans_name); 
		CRF_UNUSED(state_func); 
		memset(i2c_master_debug_state_ac, 0, 20); 
		strncpy(i2c_master_debug_state_ac, state_func, 19); 
		i2c_master_debug_state_ac[19] = '\0'; 
	#endif 
}  
