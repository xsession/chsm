/*Generated with CHSM v0.0.0 at 2023.01.22 09.47.59*/
#include "cevent.h"
#include "chsm.h"
#include "i2c_master.h"
#include "i2c_master_functions.h"


static chsm_result_ten s_scan_write(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_scan_idle(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_wr_read(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_wr_write(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_read(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_write(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst);

static chsm_result_ten s_idle(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_W_TRANSACTION:
            store_transaction_info(self, e_pst);
            i2c_master_start_tx(self, e_pst);
            return chsm_transition(self, s_write);

        case SIG_I2C_R_TRANSACTION:
            store_transaction_info(self, e_pst);
            i2c_master_start_rx(self, e_pst);
            return chsm_transition(self, s_read);

        case SIG_I2C_WR_TRANSACTION:
            store_transaction_info(self, e_pst);
            i2c_master_start_tx(self, e_pst);
            return chsm_transition(self, s_wr_write);

        case SIG_I2C_BUS_SCAN:
            scan_init(self, e_pst);
            chsm_recall(self, e_pst);
            return chsm_transition(self, s_scan_idle);

        case SIG_SYS_TICK_1ms:
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
            i2c_master_send_success_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_W_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_R_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WR_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WRITE_FAIL:
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_READ_FAIL:
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_1ms:
            i2c_1ms_callback(self, e_pst);
            break;
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_read(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_READ_SUCCESS:
            i2c_master_send_success_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_W_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_R_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WR_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WRITE_FAIL:
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_READ_FAIL:
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_1ms:
            i2c_1ms_callback(self, e_pst);
            break;
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_wr_write(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_WRITE_SUCCESS:
            i2c_master_start_rx(self, e_pst);
            return chsm_transition(self, s_wr_read);

        case SIG_I2C_W_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_R_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WR_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WRITE_FAIL:
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_READ_FAIL:
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_1ms:
            i2c_1ms_callback(self, e_pst);
            break;
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_wr_read(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_READ_SUCCESS:
            i2c_master_send_success_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_W_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_R_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WR_TRANSACTION:
            chsm_defer(self, e_pst);
            break;

        case SIG_I2C_WRITE_FAIL:
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_I2C_READ_FAIL:
            i2c_master_send_fail_response(self, e_pst);
            i2c_master_stop(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_1ms:
            i2c_1ms_callback(self, e_pst);
            break;
    }

    return chsm_ignored(self);
}

static chsm_result_ten s_scan_idle(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_I2C_W_TRANSACTION:
            store_transaction_info(self, e_pst);
            i2c_scan_start_step(self, e_pst);
            return chsm_transition(self, s_scan_write);

        case SIG_I2C_ADDRS_RELEASE:
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);

        case SIG_SYS_TICK_1ms:
            i2c_1ms_callback(self, e_pst);
            break;
    }

    if(device_addr_max_cnt(self, e_pst))
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
            i2c_master_inc_success_dev_addr(self, e_pst);
            chsm_recall(self, e_pst);
            return chsm_transition(self, s_scan_idle);

        case SIG_I2C_WRITE_FAIL:
            i2c_master_inc_dev_addr(self, e_pst);
            chsm_recall(self, e_pst);
            return chsm_transition(self, s_scan_idle);

        case SIG_SYS_TICK_1ms:
            i2c_1ms_callback(self, e_pst);
            break;
    }

    return chsm_ignored(self);
}

chsm_result_ten i2c_master_top(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            i2c_master_init(self, e_pst);
            chsm_recall(self, e_pst);
            clear_transaction_info(self, e_pst);
            return chsm_transition(self, s_idle);
    }

    return chsm_ignored(self);
}
