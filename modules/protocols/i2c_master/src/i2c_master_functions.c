#include "crf.h"
#include "cevent.h"
#include "i2c_master.h"
#include <stdio.h>


const cevent_tst success_response_st = {.sig = SIG_I2C_RESULT_SUCCESS};
const cevent_tst addr_nack_response_st = {.sig = SIG_I2C_RESULT_ADDR_NACK};
const cevent_tst data_nack_response_st = {.sig = SIG_I2C_RESULT_DATA_NACK};
const cevent_tst bus_busy_response_st = {.sig = SIG_I2C_RESULT_BUS_BUSY};

const cevent_tst sig_addr_release_st = {.sig = SIG_I2C_ADDRS_RELEASE};
const cevent_tst sig_bus_scan_st = {.sig = SIG_I2C_BUS_SCAN};

void i2c_master_init(chsm_tst *_self, const cevent_tst *e_pst)
{
    i2c_master_tst *self = (i2c_master_tst *)_self;

    self->config_st.driver_pst->q_pst = (cqueue_tst *)self;
}


void store_transaction_info(chsm_tst *_self, const cevent_tst *e_pst)
{
    i2c_master_tst *self = (i2c_master_tst *)_self;

    self->cached_tr_st = *((i2c_transaction_tst *)e_pst);
}

void clear_transaction_info(chsm_tst *self, const cevent_tst *e_pst)
{

}

void i2c_master_send_success_response(chsm_tst *_self, const cevent_tst *e_pst)
{
    i2c_master_tst *self = (i2c_master_tst *)_self;

    self->cached_tr_st.target_q_pst->put(self->cached_tr_st.target_q_pst, &success_response_st);
}

void i2c_master_send_fail_response(chsm_tst *_self, const cevent_tst *e_pst)
{
    i2c_master_tst *self = (i2c_master_tst *)_self;

    if (self->config_st.driver_pst->status_un.bit_st.busy_u16)
    {
        CRF_EMIT(&bus_busy_response_st);
        self->cached_tr_st.target_q_pst->put(self->cached_tr_st.target_q_pst, &addr_nack_response_st);
    }
    else if (self->config_st.driver_pst->status_un.bit_st.addr_nack_u16)
    {
        self->cached_tr_st.target_q_pst->put(self->cached_tr_st.target_q_pst, &addr_nack_response_st);
    }
    else if (self->config_st.driver_pst->status_un.bit_st.data_nack_u16)
    {
        self->cached_tr_st.target_q_pst->put(self->cached_tr_st.target_q_pst, &data_nack_response_st);
    }

}

void i2c_master_start_rx(chsm_tst *_self, const cevent_tst *e_pst)
{
    //printf("\n%s\n", __FUNCTION__);

    i2c_master_tst *self = (i2c_master_tst *)_self;

    i2c_transaction_tst *t_pst = &self->cached_tr_st;

    self->config_st.driver_pst->start_rx(self->config_st.driver_pst, t_pst->slave_addr_u16, t_pst->read_data_pu8, t_pst->read_cnt_u16);
}

void i2c_master_start_tx(chsm_tst *_self, const cevent_tst *e_pst)
{
    //printf("\n%s\n", __FUNCTION__);

    i2c_master_tst *self = (i2c_master_tst *)_self;

    i2c_transaction_tst *t_pst = &self->cached_tr_st;

    self->config_st.driver_pst->start_tx(self->config_st.driver_pst, t_pst->slave_addr_u16, t_pst->write_data_pu8, t_pst->write_cnt_u16);
}

void i2c_master_stop(chsm_tst *_self, const cevent_tst *e_pst)
{
    //printf("\n%s\n", __FUNCTION__);
    
    i2c_master_tst *self = (i2c_master_tst *)_self;

    self->config_st.driver_pst->stop(self->config_st.driver_pst);
}

void scan_init(chsm_tst *_self, const cevent_tst *e_pst)
{
    i2c_master_tst *self = (i2c_master_tst *)_self;

    self->scan_cnt_u8 = 1;
    *self->config_st.scan_result_au8 = 0;
}
bool device_addr_max_cnt(chsm_tst *_self, const cevent_tst *e_pst)
{
    i2c_master_tst *self = (i2c_master_tst *)_self;

    if(self->scan_cnt_u8 <= I2C_MASTER_MAX_SCAN_CNT)
    {
        self->cached_tr_st.super.sig = SIG_I2C_W_TRANSACTION;
        self->cached_tr_st.read_cnt_u16 = 0;
        self->cached_tr_st.slave_addr_u16 = self->scan_cnt_u8;
        self->cached_tr_st.write_cnt_u16 = 1;
        self->cached_tr_st.write_data_pu8 = 0;
        CRF_POST_TO_SELF(&self->cached_tr_st.super);
        return true;
    }
    else
    {
        CRF_EMIT(&sig_addr_release_st);
        return false;
    }
}
void i2c_1ms_callback(chsm_tst *_self, const cevent_tst *e_pst)
{
    i2c_master_tst *self = (i2c_master_tst *)_self;

    self->counter_u32++;
}
void i2c_master_inc_dev_addr(chsm_tst *_self, const cevent_tst *e_pst)
{
    i2c_master_tst *self = (i2c_master_tst *)_self;

    self->scan_cnt_u8++;
}
void i2c_master_inc_success_dev_addr(chsm_tst *_self, const cevent_tst *e_pst)
{
    i2c_master_tst *self = (i2c_master_tst *)_self;

    *self->config_st.scan_result_au8++ = self->scan_cnt_u8;
    self->scan_cnt_u8++;
}

void i2c_scan_start_step(chsm_tst *_self, const cevent_tst *e_pst)
{
    i2c_master_tst *self = (i2c_master_tst *)_self;
    i2c_transaction_tst *t_pst = &self->cached_tr_st;

    self->config_st.driver_pst->start_tx(self->config_st.driver_pst, t_pst->slave_addr_u16, t_pst->write_data_pu8, t_pst->write_cnt_u16);
    self->config_st.driver_pst->stop(self->config_st.driver_pst);
}
