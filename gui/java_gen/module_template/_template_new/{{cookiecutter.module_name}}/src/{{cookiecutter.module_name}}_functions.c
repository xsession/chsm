#include "{{cookiecutter.module_name}}.h"
#include "{{cookiecutter.module_name}}_regs.h"
#include "{{cookiecutter.module_name}}_functions.h"
#include "crf.h"
#include "cevent.h"
#include <stdio.h>

static const cevent_tst sig_reset_slave_comm_st = {.sig = SIG_I2C_RESET_SLAVE_COMM};
static const cevent_tst sig_reset_periph_st = {.sig = SIG_I2C_RESET_SLAVE_COMM};

void {{cookiecutter.module_name}}_init(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self =          ({{cookiecutter.module_name}}_tst *)_self;

    self->counter_u32 =         0;
    self->error_counter_u32 =   0;

    self->t_st.super.sig =      SIG_I2C_WR_TRANSACTION;
    self->t_st.slave_addr_u16 = self->config_st.address_u8;
    self->t_st.target_q_pst =   (cqueue_tst *)self;
    self->t_st.read_data_pu8 =  self->rx_buff_au8;
    self->t_st.write_data_pu8 = self->tx_buff_au8;
}

/*Increase the timer counter.*/
void {{cookiecutter.module_name}}_10ms_callback(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->counter_u32 += 10;
}

/*Increase the error counter.*/
void {{cookiecutter.module_name}}_inc_error_counter(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->error_counter_u32++;
}

void {{cookiecutter.module_name}}_reset_error_cnt(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->error_counter_u32 = 0;
}

/*Try to read the ID register from the {{cookiecutter.module_name|upper}} by sending a write-read transaction to the I2C master.*/
void {{cookiecutter.module_name}}_read_id(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->t_st.super.sig =      SIG_I2C_WR_TRANSACTION;
    self->t_st.write_cnt_u16 =  1;
    self->t_st.read_cnt_u16 =   2;
    self->tx_buff_au8[0] =      {{cookiecutter.module_name|upper}}_REG_ID;
    self->rx_buff_au8[0] =      0;
    self->rx_buff_au8[1] =      0;

    self->super.send(_self, (const cevent_tst *)(&self->t_st));
}

void {{cookiecutter.module_name}}_reset_pointer(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->t_st.super.sig =      SIG_I2C_W_TRANSACTION;
    self->t_st.write_cnt_u16 =  1;
    self->t_st.read_cnt_u16 =   0;
    self->tx_buff_au8[0] =      0;

    self->super.send(_self, (const cevent_tst *)(&self->t_st));
}

/*Send a SIG_{{cookiecutter.module_name|upper}}_OFFLINE event. This can be used to detect communication errors between the module and the I2C slave.*/
void send_offline_event(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->valid_b = false;

    TYPEOF(SIG_{{cookiecutter.module_name|upper}}_OFFLINE)* offline_pst = CRF_NEW(SIG_{{cookiecutter.module_name|upper}}_OFFLINE);

    offline_pst->id_u16 = self->config_st.id_u16;

    CRF_EMIT(offline_pst);
}

/*Send a SIG_{{cookiecutter.module_name|upper}}_ONLINE event. This can be used to detect successful initialization.*/
void send_online_event(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->valid_b = true;
    TYPEOF(SIG_{{cookiecutter.module_name|upper}}_ONLINE)* online_pst = CRF_NEW(SIG_{{cookiecutter.module_name|upper}}_ONLINE);

    online_pst->id_u16 = self->config_st.id_u16;

    CRF_EMIT(online_pst);
}

/*Reset the timer counter.*/
void {{cookiecutter.module_name}}_reset_timer(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);
    
    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->counter_u32 = 0;
}

/*Send a read transaction to the {{cookiecutter.module_name|upper}}.*/
void {{cookiecutter.module_name}}_start_read(chsm_tst *_self, const cevent_tst *e_pst)
{    
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->t_st.super.sig =      SIG_I2C_R_TRANSACTION;
    self->t_st.write_cnt_u16 =  0;
    self->t_st.read_cnt_u16 =   2;
    self->rx_buff_au8[0] =      0;
    self->rx_buff_au8[1] =      0;

    self->super.send(_self, (const cevent_tst *)(&self->t_st));
}

void {{cookiecutter.module_name}}_set_full_powerdown(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->t_st.super.sig =      SIG_I2C_W_TRANSACTION;
    self->t_st.write_cnt_u16 =  2;
    self->t_st.read_cnt_u16 =   0;
    self->tx_buff_au8[0] =      {{cookiecutter.module_name|upper}}_REG_CONFIG;
    self->tx_buff_au8[1] =      {{cookiecutter.module_name|upper}}_POWER_OFF;

    self->super.send(_self, (const cevent_tst *)(&self->t_st));
}
void {{cookiecutter.module_name}}_set_full_powerup(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->t_st.super.sig =      SIG_I2C_W_TRANSACTION;
    self->t_st.write_cnt_u16 =  2;
    self->t_st.read_cnt_u16 =   0;
    self->tx_buff_au8[0] =      {{cookiecutter.module_name|upper}}_REG_CONFIG;
    self->tx_buff_au8[1] =      {{cookiecutter.module_name|upper}}_POWER_ON;

    self->super.send(_self, (const cevent_tst *)(&self->t_st));
}

bool {{cookiecutter.module_name}}_wait_cnt(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);
    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    if(self->wait_cnt_u16++ >= {{cookiecutter.module_name|upper}}_WAIT_CNT)
    {
        self->wait_cnt_u16 = 0;
        return true;
    }
    else
    {
       return false;
    }
}

bool {{cookiecutter.module_name}}_inc_wait_cnt(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);
    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;
    self->wait_cnt_u16++;
    return false;
}

void {{cookiecutter.module_name}}_init_wait(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);
    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;
//    self->wait_cnt_u16 = 0;
}

void {{cookiecutter.module_name}}_get_resolution(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->t_st.super.sig =      SIG_I2C_WR_TRANSACTION;
    self->t_st.write_cnt_u16 =  1;
    self->t_st.read_cnt_u16 =   1;
    self->tx_buff_au8[0] =      {{cookiecutter.module_name|upper}}_REG_CTRLSTATUS;
    self->rx_buff_au8[1] =      0;

    self->super.send(_self, (const cevent_tst *)(&self->t_st));
}

void {{cookiecutter.module_name}}_update_resolution(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);
    
    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;
    uint16_t reg_u16 = self->rx_buff_au8[0];
    {{cookiecutter.module_name}}_resolution_t resolution = {{cookiecutter.module_name|upper}}_RESOLUTION_14BIT;

    self->resolution_u16 = (reg_u16 & {{cookiecutter.module_name|upper}}_MASK_RESOLUTION) | resolution;
}

void {{cookiecutter.module_name}}_set_resolution(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    self->t_st.super.sig =      SIG_I2C_W_TRANSACTION;
    self->t_st.write_cnt_u16 =  2;
    self->t_st.read_cnt_u16 =   0;
    self->tx_buff_au8[0] =      {{cookiecutter.module_name|upper}}_REG_CTRLSTATUS;
    self->tx_buff_au8[1] =      {{cookiecutter.module_name|upper}}_RESOLUTION_14BIT;

    self->super.send(_self, (const cevent_tst *)(&self->t_st));
}

/*Update the temperature display and send an event with the new value.*/
void {{cookiecutter.module_name}}_update_temp(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);
    
    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    {{cookiecutter.module_name}}_temp_tst* temp_pst = CRF_NEW(SIG_{{cookiecutter.module_name|upper}}_TEMP); //{{cookiecutter.module_name}}_temp_tst

    if (NULL == temp_pst) return;

    // Compile the value of the Temperature Data Register into a variable
    double tdr_reg_u32 = ((((self->rx_buff_au8[0] << 8) | self->rx_buff_au8[1]) >> 2) * 0.03125);
    int32_t temp_c_i32 = tdr_reg_u32*1000;

    temp_pst->super.sig = SIG_{{cookiecutter.module_name|upper}}_TEMP;
    temp_pst->temp_C_i32 = temp_c_i32;
    temp_pst->id_u16 = self->config_st.id_u16;

    self->temp_C_i32 = temp_c_i32;
    self->valid_b = true;
    
    self->super.send(_self, (const cevent_tst *)temp_pst);

    if (self->error_counter_u32)
    {
        self->error_counter_u32--;
    }
}

/*True, if the response data is equal to 0x190. See {{cookiecutter.module_name|upper}} datasheet section: 7.5.1.7 Identification Register*/
bool {{cookiecutter.module_name}}_id_match(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);
    
    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    // Compile the value of the Identification Register into a variable
    uint16_t idr_reg_u16 = self->rx_buff_au8[0];
    idr_reg_u16 <<= 8;
    idr_reg_u16 |= self->rx_buff_au8[1];

    return {{cookiecutter.module_name|upper}}_ID_REG_VALUE == idr_reg_u16;
}

/*Return true, if the error counter is greater or equal then the parameter.*/
bool {{cookiecutter.module_name}}_error_count(chsm_tst *_self, const cevent_tst *e_pst, uint16_t error_cnt_threshold_u16)
{   
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    return self->error_counter_u32 >= error_cnt_threshold_u16;
}

bool {{cookiecutter.module_name}}_timeout(chsm_tst *_self, const cevent_tst *e_pst, uint32_t timeout_u32)
{  
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    return self->counter_u32 >= timeout_u32;
}

void {{cookiecutter.module_name}}_unplugged(chsm_tst *_self, const cevent_tst *e_pst)
{
    {{cookiecutter.module_name}}_debug_log_func(_self,e_pst,"",__FUNCTION__);

    {{cookiecutter.module_name}}_tst*   self = ({{cookiecutter.module_name}}_tst *)_self;

    CRF_EMIT(&sig_reset_slave_comm_st);
}

