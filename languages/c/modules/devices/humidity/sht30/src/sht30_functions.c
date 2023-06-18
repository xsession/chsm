#include "sht30_functions.h"
#include "sht30.h"
#include "sht30_regs.h"
#include <stdbool.h>
#include <stdint.h>

static const cevent_tst sig_init_st = { .sig = C_SIG_INIT };

// ------- Static functions -------

static float sht30_calc_temperature(uint16_t rawValue)
{
    // calculate temperature [Celsius]
    // T = -45 + 175 * rawValue / (2^16-1)
    return 175.0f * (float)rawValue / 65535.0f - 45.0f;
}

static float sht30_calc_humidity(uint16_t rawValue)
{
    // calculate relative humidity [%RH]
    // RH = rawValue / (2^16-1) * 100
    return 100.0f * (float)rawValue / 65535.0f;
}

static uint16_t sht30_u8_to_u16(uint8_t low, uint8_t high)
{
    uint16_t high_u16 = high;
    uint16_t low_u16  = low;
    high_u16 <<= 8;

    return (high_u16 | low_u16);
}

// ------- In statemachine used functions -------

void sht30_i2c_power_state(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    CRF_SIG_VAR(SIG_SHT30_I2C_POWER_STATE, sig_i2c_power_state_st, e_pst);
    if (sig_i2c_power_state_st->power_state_on_u16 == 0)
    {
        CRF_POST_TO_SELF(&sig_init_st);
    }
}

static void sht30_set_state(chsm_tst *self, const cevent_tst *e_pst, uint16_t state_id)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    sht30_tst *_self = (sht30_tst *)self;

    _self->state_id_en = state_id;
}

void sht30_init(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    sht30_tst *_self = (sht30_tst *)self;

    _self->counter_u32             = 0;
    _self->error_counter_u32       = 0;
    _self->state_id_en             = 0;
    _self->temp_C_i32              = 0;
    _self->humidity_i32            = 0;
    _self->serial_num.serialNumber = 0;

    _self->t_st.super.sig      = SIG_I2C_W_TRANSACTION;
    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.target_q_pst   = (cqueue_tst *)self;
    _self->t_st.read_data_pu8  = _self->rx_buff_au8;
    _self->t_st.write_data_pu8 = _self->tx_buff_au8;

    CRF_EMIT(&_self->t_st);
}

void sht30_send_online_event(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    sht30_tst *_self = (sht30_tst *)self;

    TYPEOF(SIG_SHT30_ONLINE) *online_pst = CRF_NEW(SIG_SHT30_ONLINE);

    online_pst->id_u16 = _self->config_st.id_u16;

    CRF_EMIT(online_pst);
}

void sht30_send_offline_event(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    sht30_tst *_self = (sht30_tst *)self;
    _self->valid_b   = false;

    TYPEOF(SIG_SHT30_OFFLINE) *offline_pst = CRF_NEW(SIG_SHT30_OFFLINE);

    offline_pst->id_u16 = _self->config_st.id_u16;

    CRF_EMIT(offline_pst);
}

void sht30_10ms_callback(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    sht30_tst *_self = (sht30_tst *)self;

    _self->counter_u32 += 10;
}

void sht30_inc_error_counter(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    sht30_tst *_self = (sht30_tst *)self;
    _self->error_counter_u32++;
}

void sht30_measurement_reg_write(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    CRF_UNUSED(e_pst);
    sht30_tst *_self = (sht30_tst *)self;

    sht30_commands_ten command   = SHT30_CMD_MEAS_CLOCKSTR_L;
    uint8_t            command_l = command & 0xff;
    uint8_t            command_h = command >> 8;

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_WR_TRANSACTION;
    _self->t_st.write_cnt_u16  = 2;
    _self->t_st.read_cnt_u16   = 6;
    _self->tx_buff_au8[0]      = command_h;
    _self->tx_buff_au8[1]      = command_l;

    CRF_EMIT(&_self->t_st);
}

void sht30_init_measurement_reg_write(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    CRF_UNUSED(e_pst);
    sht30_tst *_self = (sht30_tst *)self;
    // sht30_set_state(self, e_pst, S_INIT_READ);

    sht30_commands_ten command   = SHT30_CMD_MEAS_CLOCKSTR_L;
    uint8_t            command_l = command & 0xff;
    uint8_t            command_h = command >> 8;

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_W_TRANSACTION;
    _self->t_st.write_cnt_u16  = 2;
    _self->t_st.read_cnt_u16   = 0;
    _self->tx_buff_au8[0]      = command_h;
    _self->tx_buff_au8[1]      = command_l;

    CRF_EMIT(&_self->t_st);
}
void sht30_measurement_reg_read(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    CRF_UNUSED(e_pst);
    sht30_tst *_self = (sht30_tst *)self;
    sht30_set_state(self, e_pst, S_MEASUREMENT_INIT_READ);

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_R_TRANSACTION;
    _self->t_st.write_cnt_u16  = 0;
    _self->t_st.read_cnt_u16   = 6;

    CRF_EMIT(&_self->t_st);
}

void sht30_get_status(chsm_tst *_self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(_self,e_pst,"",__FUNCTION__);
    CRF_UNUSED(e_pst);
    volatile sht30_tst *self = (sht30_tst *)_self;

    volatile sht30_commands_ten command   = SHT30_CMD_READ_STATUS;
    volatile uint8_t            command_l = command & 0xff;
    volatile uint8_t            command_h = command >> 8;

    self->t_st.slave_addr_u16 = self->config_st.address_u8;
    self->t_st.super.sig      = SIG_I2C_WR_TRANSACTION;
    self->t_st.write_cnt_u16  = 2;
    self->t_st.read_cnt_u16   = 3;
    self->tx_buff_au8[0]      = command_h;
    self->tx_buff_au8[1]      = command_l;

    CRF_EMIT(&self->t_st);
//    self->super.send(_self, (const cevent_tst *)(&self->t_st));
}

void sht30_unplugged(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    CRF_UNUSED(e_pst);
    sht30_tst *_self = (sht30_tst *)self;
//    sht30_set_state(_self, e_pst, S_UNPLUGGED);
}

void sht30_clear_status_reg(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    CRF_UNUSED(e_pst);
    sht30_tst *_self = (sht30_tst *)self;
    sht30_set_state(self, e_pst, S_CLEAR_STATUS_REG);

    sht30_commands_ten command   = SHT30_CMD_CLEAR_STATUS;
    uint8_t            command_l = command & 0xff;
    uint8_t            command_h = command >> 8;

    _self->t_st.slave_addr_u16 = _self->config_st.address_u8;
    _self->t_st.super.sig      = SIG_I2C_W_TRANSACTION;
    _self->t_st.write_cnt_u16  = 2;
    _self->t_st.read_cnt_u16   = 0;
    _self->tx_buff_au8[0]      = command_h;
    _self->tx_buff_au8[1]      = command_l;

    CRF_EMIT(&_self->t_st);
}

void sht30_check_status(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    CRF_UNUSED(e_pst);
    sht30_tst *_self  = (sht30_tst *)self;
    uint16_t   status = sht30_u8_to_u16(_self->rx_buff_au8[1], _self->rx_buff_au8[0]);
}

void sht30_update_data(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    sht30_tst *_self            = (sht30_tst *)self;
    uint16_t   raw_temp_u16     = sht30_u8_to_u16(_self->rx_buff_au8[1], _self->rx_buff_au8[0]);
    uint16_t   raw_humidity_u16 = sht30_u8_to_u16(_self->rx_buff_au8[4], _self->rx_buff_au8[3]);

    uint32_t humidity = sht30_calc_humidity(raw_humidity_u16) * 1000;
    uint32_t temp     = sht30_calc_temperature(raw_temp_u16) * 1000;

    _self->temp_C_i32   = temp;
    _self->humidity_i32 = humidity;

    TYPEOF(SIG_SHT30_READ_SAMPLE) *read_sample_pst = CRF_NEW(SIG_SHT30_READ_SAMPLE);

    read_sample_pst->temp_C_i32   = temp;
    read_sample_pst->humidity_i32 = humidity;

    CRF_EMIT(read_sample_pst);
}

void sht30_reset_timer(chsm_tst *self, const cevent_tst *e_pst)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    sht30_tst *_self = (sht30_tst *)self;

//    sht30_set_state(_self, e_pst, S_IDLE);

    _self->counter_u32 = 0;
}

bool sht30_timeout(chsm_tst *self, const cevent_tst *e_pst, uint32_t timeout_u32)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    sht30_tst *_self = (sht30_tst *)self;

    return _self->counter_u32 >= timeout_u32;
}

bool sht30_error_count(chsm_tst *self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16)
{
    sht30_debug_log_func(self,e_pst,"",__FUNCTION__);
    sht30_tst *_self = (sht30_tst *)self;

    return _self->error_counter_u32 >= error_cnt_threshold_u16;
}
