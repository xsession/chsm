#include "tmp468_functions.h"
#include "tmp468.h"
#include "tmp468_regs.h"
#include <stdio.h>

static const cevent_tst sig_init_st = { .sig = C_SIG_INIT };

#define DIV_ROUND_CLOSEST(n, d) ((((n) < 0) ^ ((d) < 0)) ? (((n) - (d) / 2) / (d)) : (((n) + (d) / 2) / (d)))

// ------- Static functions -------

static uint32_t tmp468_raw2celsius(uint32_t raw_temp)
{
    return ((double)raw_temp * 100) / 128;
}

static uint16_t tmp468_u8_to_u16(uint8_t low, uint8_t high)
{
    uint16_t high_u16 = high;
    uint16_t low_u16  = low;
    high_u16 <<= 8;

    return (high_u16 | low_u16);
}

// ------- In statemachine used functions -------

void tmp468_i2c_power_state(chsm_tst *self, const cevent_tst *e_pst)
{
    CRF_SIG_VAR(SIG_TMP468_I2C_POWER_STATE, sig_i2c_power_state_st, e_pst);
    if (sig_i2c_power_state_st->power_state_on_u16 == 0)
    {
        CRF_POST_TO_SELF(&sig_init_st);
    }
}

void tmp468_set_state(chsm_tst *self, const cevent_tst *e_pst, uint16_t state_id)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->state_id_en          = state_id;
    volatile uint32_t fault_cnt = self->event_q_st.fault_cnt;
}

void tmp468_init(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->counter_u32         = 0;
    _self->reg_inc_counter_u16 = 0;
    _self->error_counter_u32   = 0;
    _self->state_id_en         = 0;
    _self->temp_C_i32[0]       = 0;
    _self->temp_C_i32[1]       = 0;
    _self->temp_C_i32[2]       = 0;
    _self->temp_C_i32[3]       = 0;

    _self->t_st.super.sig      = SIG_I2C_W_TRANSACTION;
    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.target_q_pst   = (cqueue_tst *)self;
    _self->t_st.read_data_pu8  = _self->rx_buff_au8;
    _self->t_st.write_data_pu8 = _self->tx_buff_au8;

    tmp468_set_state(self, e_pst, S_TMP468);
}

void tmp468_10ms_callback(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->counter_u32 += 10;
    _self->counter_status_u32 += 10;
}

void tmp468_send_offline_event(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->valid_b = false;

    TYPEOF(SIG_TMP468_OFFLINE) *offline_pst = CRF_NEW(SIG_TMP468_OFFLINE);

    offline_pst->id_u16 = _self->config_st.id_u16;

    CRF_EMIT(offline_pst);
}

void tmp468_send_online_event(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    tmp468_set_state(self, e_pst, S_ONLINE);

    TYPEOF(SIG_TMP468_ONLINE) *online_pst = CRF_NEW(SIG_TMP468_ONLINE);

    online_pst->id_u16 = _self->config_st.id_u16;

    CRF_EMIT(online_pst);
}

void tmp468_inc_error_counter(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->error_counter_u32++;
}

void tmp468_unlock_regs(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;
    //    tmp468_set_state(self, e_pst, S_SET_N_FACTORS);
    uint16_t command   = TMP468_LOCK_UNLOCK_VALUE;
    uint8_t  command_l = command & 0xff;
    uint8_t  command_h = command >> 8;

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_W_TRANSACTION;
    _self->t_st.write_cnt_u16  = 3;
    _self->tx_buff_au8[0]      = TMP468_LOCK;
    _self->tx_buff_au8[1]      = command_h;
    _self->tx_buff_au8[2]      = command_l;

    CRF_EMIT(&_self->t_st);
}

void tmp468_set_n_factor1(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_W_TRANSACTION;
    _self->t_st.write_cnt_u16  = 3;
    _self->tx_buff_au8[0]      = TMP468_REMOTE1_TEMP_N_FACTOR;
    _self->tx_buff_au8[1]      = 0x01;
    _self->tx_buff_au8[2]      = 0x00;

    CRF_EMIT(&_self->t_st);
}
void tmp468_set_n_factor2(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_W_TRANSACTION;
    _self->t_st.write_cnt_u16  = 3;
    _self->tx_buff_au8[0]      = TMP468_REMOTE2_TEMP_N_FACTOR;
    _self->tx_buff_au8[1]      = 0x01;
    _self->tx_buff_au8[2]      = 0x00;

    CRF_EMIT(&_self->t_st);
}
void tmp468_set_n_factor3(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_W_TRANSACTION;
    _self->t_st.write_cnt_u16  = 3;
    _self->tx_buff_au8[0]      = TMP468_REMOTE3_TEMP_N_FACTOR;
    _self->tx_buff_au8[1]      = 0x01;
    _self->tx_buff_au8[2]      = 0x00;

    CRF_EMIT(&_self->t_st);
}
void tmp468_set_n_factor4(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_W_TRANSACTION;
    _self->t_st.write_cnt_u16  = 3;
    _self->tx_buff_au8[0]      = TMP468_REMOTE4_TEMP_N_FACTOR;
    _self->tx_buff_au8[1]      = 0x00;
    _self->tx_buff_au8[2]      = 0x00;

    CRF_EMIT(&_self->t_st);
}

void tmp468_read_id(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;
    tmp468_set_state(self, e_pst, S_READ_ID_REG);

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_WR_TRANSACTION;
    _self->t_st.write_cnt_u16  = 1;
    _self->t_st.read_cnt_u16   = 2;
    _self->tx_buff_au8[0]      = TMP468_DEV_ID;
    _self->rx_buff_au8[0]      = 0;
    _self->rx_buff_au8[1]      = 0;

    _self->super.send((chsm_tst *)_self, (const cevent_tst *)(&_self->t_st));
    CRF_EMIT(&_self->t_st);
}

bool tmp468_id_match(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    // Compile the value of the Identification Register into a variable
    uint16_t idr_reg_u16 = _self->rx_buff_au8[0];
    idr_reg_u16 <<= 8;
    idr_reg_u16 |= _self->rx_buff_au8[1];

    uint16_t temp_u16 = tmp468_u8_to_u16(_self->rx_buff_au8[1], _self->rx_buff_au8[0]);
    return TMP468_DEV_ID_VALUE == idr_reg_u16;
}

void tmp468_reset_timer(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;
    tmp468_set_state(self, e_pst, S_IDLE);

    _self->counter_u32 = 0;
}

void tmp468_read_temp(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    tmp468_set_state(self, e_pst, S_READ_TEMP);

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_WR_TRANSACTION;
    _self->t_st.write_cnt_u16  = 1;
    _self->t_st.read_cnt_u16   = 8;
    _self->tx_buff_au8[0]      = TMP468_REMOTE1_TEMP_AUTO_INC_PTR;
    _self->rx_buff_au8[0]      = 0;
    _self->rx_buff_au8[1]      = 0;

    CRF_EMIT(&_self->t_st);
}

void tmp468_update_temp(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self                      = (tmp468_tst *)self;
    TYPEOF(SIG_TMP468_READ_TEMP) *temp_pst = CRF_NEW(SIG_TMP468_READ_TEMP);

    temp_pst->temp_C_i32[0] = tmp468_raw2celsius(tmp468_u8_to_u16(_self->rx_buff_au8[1], _self->rx_buff_au8[0]));
    temp_pst->temp_C_i32[1] = tmp468_raw2celsius(tmp468_u8_to_u16(_self->rx_buff_au8[3], _self->rx_buff_au8[2]));
    temp_pst->temp_C_i32[2] = tmp468_raw2celsius(tmp468_u8_to_u16(_self->rx_buff_au8[5], _self->rx_buff_au8[4]));
    temp_pst->temp_C_i32[3] = tmp468_raw2celsius(tmp468_u8_to_u16(_self->rx_buff_au8[7], _self->rx_buff_au8[6]));

    for (uint16_t i = 0; i < TMP468_READ_REG_NUM; i++)
    {
        _self->temp_C_i32[i] = temp_pst->temp_C_i32[i];
    }

    _self->valid_b = true;

    CRF_EMIT(temp_pst);

    if (_self->error_counter_u32)
    {
        _self->error_counter_u32--;
    }
}

void tmp468_increment_reg_index(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->reg_inc_counter_u16++;
}

void tmp468_read_sensor_status(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;
    tmp468_set_state(self, e_pst, S_READ_SENSOR_STATUS);

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_WR_TRANSACTION;
    _self->t_st.write_cnt_u16  = 1;
    _self->t_st.read_cnt_u16   = 2;
    _self->tx_buff_au8[0]      = TMP468_REMOTE_CH_OPEN_STATUS;
    _self->rx_buff_au8[0]      = 0;
    _self->rx_buff_au8[1]      = 0;

    CRF_EMIT(&_self->t_st);
}

void tmp468_update_status(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    uint16_t status_u16 = tmp468_u8_to_u16(_self->rx_buff_au8[1], _self->rx_buff_au8[0]);

    TYPEOF(SIG_TMP468_READ_SENSOR_STATUS) *read_status_pst = CRF_NEW(SIG_TMP468_READ_SENSOR_STATUS);

    _self->status_tun.all           = status_u16;
    read_status_pst->status_tun.all = status_u16;

    CRF_EMIT(read_status_pst);

    tmp468_reset_status_timer(self, e_pst);
}

bool tmp468_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    return _self->counter_u32 >= timeout_u32;
}
bool tmp468_status_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    return _self->counter_status_u32 >= timeout_u32;
}

void tmp468_reset_status_timer(chsm_tst *self, const cevent_tst *e_pst)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    _self->counter_status_u32 = 0;
}

bool tmp468_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16)
{
    tmp468_tst *_self = (tmp468_tst *)self;

    return _self->error_counter_u32 >= error_cnt_threshold_u16;
}
