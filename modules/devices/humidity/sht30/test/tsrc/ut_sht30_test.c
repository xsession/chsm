/*
 * ut_sht30_test.c
 *
 *  Created on: 2020. dec. 30.
 *      Author: jszeman
 */

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
i2c_driver_if_tst *    drv_pst = (i2c_driver_if_tst *)&drv_mock_st;

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
    // printf("\n%s\n", __FUNCTION__);
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

    sht30_st.config_st = (sht30_cfg_tst) { .address_u8 = SHT30_ADDR_ADDR_PIN_LOW, .id_u16 = 0xabcd, .period_ms_u16 = SHT30_READ_PERIOD_VALUE, .max_error_cnt_u16 = SHT30_MAX_ERROR_COUNT_VALUE };

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
}

/* go_offline:
 * Check that after reading temperature data fails for more than SHT30_MAX_ERROR_COUNT times
 * the state machine goes offline and invalidates the temperature data.
 */
TEST(sht30, go_offline)
{
    // init test
    i2c_mock_slave_device_tst dev_st = { .address_u8 = SHT30_ADDR_ADDR_PIN_LOW, .nack_idx_u16 = 20, .tx_data_au8 = { 0x01, 0x90, 0xf3, 0x80, 0x0c, 0x80 } };

    const sht30_sample_tst *e_pst;
    const sht30_status_tst *s_pst;

    drv_mock_st.slave_pst = &dev_st;

    // wait 10ms
    tick_ms(100);

    // get signals
    // s_pst = (sht30_status_tst*)q_st.get(&q_st);
    // TEST_ASSERT(s_pst);
    // TEST_ASSERT_EQUAL(SIG_SHT30_ONLINE, s_pst->super.sig);

    e_pst = (sht30_sample_tst *)q_st.get(&q_st);
    TEST_ASSERT(e_pst);
    TEST_ASSERT_EQUAL(SIG_SHT30_READ_SAMPLE, e_pst->super.sig);
    TEST_ASSERT_EQUAL(-25, e_pst->temp_C_i32);

    // wait SHT30_READ_PERIOD+1
    tick_ms(SHT30_READ_PERIOD + 1);

    // get
    e_pst = (sht30_sample_tst *)q_st.get(&q_st);
    TEST_ASSERT(e_pst);
    TEST_ASSERT_EQUAL(SIG_SHT30_READ_SAMPLE, e_pst->super.sig);
    TEST_ASSERT_EQUAL(25, e_pst->temp_C_i32);

    TEST_ASSERT_TRUE(sht30_st.valid_b);

    dev_st.address_u8 = 0x13;

    tick_ms((SHT30_READ_PERIOD + 1) * SHT30_MAX_ERROR_COUNT + 1);

    s_pst = (sht30_status_tst *)q_st.get(&q_st);
    TEST_ASSERT(s_pst);
    TEST_ASSERT_EQUAL(SIG_SHT30_OFFLINE, s_pst->super.sig);

    TEST_ASSERT_FALSE(sht30_st.valid_b);
}

TEST(sht30, get_status)
{
    // init test
    i2c_mock_slave_device_tst dev_st = { .address_u8 = SHT30_ADDR_ADDR_PIN_LOW, .nack_idx_u16 = 20, .tx_data_au8 = { 0x01, 0x90, 0xf3, 0x80, 0x0c, 0x80 } };

    const sht30_sample_tst *e_pst;
    const sht30_status_tst *s_pst;

    drv_mock_st.slave_pst = &dev_st;
    TEST_ASSERT(false);
}

// /* triggered_read:
//  * Check that it is possible to trigger a read before the read period timeout elapses.
//  */
// TEST(sht30, triggered_read)
// {
// 	    i2c_mock_slave_device_tst dev_st = {
//         .address_u8 = 0x12,
//         .nack_idx_u16 = 20,
// 		.tx_data_au8 = {0x01, 0x90, 0xf3, 0x80, 0x0c, 0x80}
//     };

// 	const cevent_tst read_event = {.sig = SIG_SHT30_READ};

// 	const sht30_temp_tst* e_pst;
// 	const sht30_status_tst* s_pst;

//     drv_mock_st.slave_pst = &dev_st;

// 	tick_ms(10);

// 	s_pst = (sht30_status_tst*)q_st.get(&q_st);
// 	TEST_ASSERT(s_pst);
// 	TEST_ASSERT_EQUAL(SIG_SHT30_ONLINE, s_pst->super.sig);

// 	e_pst = (sht30_temp_tst*)q_st.get(&q_st);
// 	TEST_ASSERT(e_pst);
// 	TEST_ASSERT_EQUAL(SIG_SHT30_READ_SAMPLE, e_pst->super.sig);
// 	TEST_ASSERT_EQUAL(-25, e_pst->temp_C_i32);

// 	tick_ms(1);

// 	CRF_POST(&read_event, &sht30_st);

// 	tick_ms(10);

// 	uint8_t expected_au8[4] = {7, 0, 0, 0};
//     TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_au8, dev_st.rx_data_au8, 4);
// 	TEST_ASSERT_EQUAL(2, dev_st.rx_idx_u16);
// 	TEST_ASSERT_EQUAL(6, dev_st.tx_idx_u16);

// 	e_pst = (sht30_temp_tst*)q_st.get(&q_st);
// 	TEST_ASSERT(e_pst);
// 	TEST_ASSERT_EQUAL(SIG_SHT30_READ_SAMPLE, e_pst->super.sig);
// 	TEST_ASSERT_EQUAL(25, e_pst->temp_C_i32);
// }

TEST_GROUP_RUNNER(sht30)
{
    RUN_TEST_CASE(sht30, init);
    // RUN_TEST_CASE(sht30, read_temp_twice);
    // RUN_TEST_CASE(sht30, read_id_retry);
    // RUN_TEST_CASE(sht30, read_id_retry_bad_id);
    // RUN_TEST_CASE(sht30, go_offline);
    // RUN_TEST_CASE(sht30, get_status);
    // RUN_TEST_CASE(sht30, triggered_read);
    // RUN_TEST_CASE(sht30, init);
}
