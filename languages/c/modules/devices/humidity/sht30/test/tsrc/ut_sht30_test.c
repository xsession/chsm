
#include "cbits.h"
#include "cevent.h"
#include "crf.h"
#include "i2c_master.h"
#include "sht30.h"
#include "sht30_regs.h"
#include "sys_if.h"
#include "unity_fixture.h"
#include "ut_i2c_driver_mock.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TEST_GROUP(sht30);

ut_i2c_driver_mock_tst drv_mock_st;
i2c_driver_if_tst     *drv_pst = (i2c_driver_if_tst *)&drv_mock_st;

const cevent_tst *i2c_master_events_apst[8];
i2c_master_tst    i2c_master_st;

const cevent_tst *sht30_events_apst[8];
sht30_tst         sht30_st;

sht30_tst *self = &sht30_st; // This is necessary for macros like SHT30_READ_PERIOD to work here

chsm_tst *hsm_apst[] = { (chsm_tst *)(&i2c_master_st), (chsm_tst *)(&sht30_st), NULL };

uint8_t   pool_buff_au8[1024];
cpool_tst pool_ast[1];

crf_tst crf;

const cevent_tst *events_apst[4];
cqueue_tst        q_st;

const cevent_tst tick_1ms_st = { .sig = SIG_SYS_TICK_1ms };

static void tick_ms(uint32_t tick_cnt_u32)
{
    while (tick_cnt_u32--)
    {
        drv_mock_st.tick(&drv_mock_st);

        CRF_POST(&tick_1ms_st, &sht30_st);

        while (CRF_STEP())
        {
            printf("|");
        }
    }
}

void sht30_send(chsm_tst *self, const cevent_tst *e_pst)
{
    switch (e_pst->sig)
    {
        case SIG_SHT30_READ_SAMPLE:
            CRF_POST(e_pst, &q_st);
            break;
        case SIG_SHT30_READ_STATUS:
            CRF_POST(e_pst, &q_st);
            break;
        case SIG_SHT30_ONLINE:
            CRF_POST(e_pst, &q_st);
            break;
        case SIG_SHT30_OFFLINE:
            CRF_POST(e_pst, &q_st);
            break;
        case SIG_SHT30_I2C_POWER_STATE:
            CRF_POST(e_pst, &q_st);
            break;

        default:
            CRF_POST(e_pst, &i2c_master_st);
    }
}

TEST_SETUP(sht30)
{
    memset(events_apst, 0, sizeof events_apst);
    memset(&q_st, 0, sizeof q_st);
    memset(&drv_mock_st, 0, sizeof drv_mock_st);
    memset(&i2c_master_st, 0, sizeof i2c_master_st);
    memset(&crf, 0, sizeof crf);
    memset(&i2c_master_events_apst, 0, sizeof i2c_master_events_apst);
    memset(&pool_buff_au8, 0, sizeof pool_buff_au8);
    memset(&pool_ast, 0, sizeof pool_ast);
    memset(&sht30_st, 0, sizeof sht30_st);

    cpool_init(pool_ast + 0, pool_buff_au8, 24, 16);

    cqueue_init(&q_st, events_apst, 4);

    ut_i2c_driver_mock_init(&drv_mock_st);

    i2c_master_st.config_st.driver_pst = drv_pst;
    chsm_ctor(&i2c_master_st.super, i2c_master_top, i2c_master_events_apst, 4, 4);
    chsm_ctor(&sht30_st.super, sht30_top, sht30_events_apst, 8, 0);

    sht30_st.config_st = (sht30_cfg_tst) { 
        .address_u8 = SHT30_ADDR_ADDR_PIN_LOW, 
        .id_u16 = 0xabcd, 
        .period_ms_u16 = SHT30_READ_PERIOD_VALUE, 
        .max_error_cnt_u16 = SHT30_MAX_ERROR_COUNT_VALUE };

    CRF_SEND_FUNC(&sht30_st) = sht30_send;

    crf_init(&crf, hsm_apst, pool_ast, 1);
    chsm_init((chsm_tst *)&i2c_master_st);
    chsm_init((chsm_tst *)&sht30_st);
}

TEST_TEAR_DOWN(sht30)
{
}

/*
 * Just call setup
 */
TEST(sht30, init)
{
    TEST_ASSERT(1);
}

TEST(sht30, get_status)
{
    i2c_mock_slave_device_tst dev_st = { 
        .address_u8 = SHT30_ADDR_ADDR_PIN_LOW, 
        .nack_idx_u16 = 20, 
        .tx_data_au8 = { 0x80, 0x10, 0xe1 } 
        };

    const sht30_sample_tst *s_pst;

    drv_mock_st.slave_pst = &dev_st;
}

TEST(sht30, read_measurement_in_online_state)
{
    
}

TEST(sht30, unplug_check)
{
    i2c_mock_slave_device_tst dev_st = { 
        .address_u8 = SHT30_ADDR_ADDR_PIN_LOW, 
        .nack_idx_u16 = 20, 
        .tx_data_au8 = { 0x80, 0x10, 0xe1 } 
        };

    const sht30_sample_tst *s_pst;

    drv_mock_st.slave_pst = &dev_st;

    tick_ms(14);

    s_pst = (sht30_sample_tst *)q_st.get(&q_st);
    TEST_ASSERT(s_pst);
    // TEST_ASSERT_EQUAL(SIG_SHT30_READ_SAMPLE, s_pst->super.sig);
    // TEST_ASSERT_EQUAL_HEX(0x5566, s_pst->sample_u16);
}

TEST_GROUP_RUNNER(sht30)
{
    RUN_TEST_CASE(sht30, init);
    RUN_TEST_CASE(sht30, unplug_check);
}
