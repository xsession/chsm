#include "cbits.h"
#include "cevent.h"
#include "crf.h"
#include "i2c_master.h"
#include "sys_if.h"
#include "tmp468.h"
#include "tmp468_regs.h"
#include "unity_fixture.h"
#include "ut_i2c_driver_mock.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TEST_GROUP(tmp468);

ut_i2c_driver_mock_tst drv_mock_st;
i2c_driver_if_tst *    drv_pst = (i2c_driver_if_tst *)&drv_mock_st;

const cevent_tst *i2c_master_events_apst[8];
i2c_master_tst    i2c_master_st;

const cevent_tst *tmp468_events_apst[8];
tmp468_tst        tmp468_st;

tmp468_tst *self = &tmp468_st; // This is necessary for macros like TMP468_READ_PERIOD to work here

chsm_tst *hsm_apst[] = { (chsm_tst *)(&i2c_master_st), (chsm_tst *)(&tmp468_st), NULL };

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

        CRF_POST(&tick_1ms_st, &tmp468_st);

        while (CRF_STEP())
        {
            printf("|");
        }
    }
}

void tmp468_send(chsm_tst *self, const cevent_tst *e_pst)
{
    // printf("\n%s\n", __FUNCTION__);
    switch (e_pst->sig)
    {
        case SIG_TMP468_TEMP:
        case SIG_TMP468_ONLINE:
        case SIG_TMP468_OFFLINE:
            CRF_POST(e_pst, &q_st);
            break;

        default:
            CRF_POST(e_pst, &i2c_master_st);
    }
}

TEST_SETUP(tmp468)
{
    memset(events_apst, 0, sizeof events_apst);
    memset(&q_st, 0, sizeof q_st);
    memset(&drv_mock_st, 0, sizeof drv_mock_st);
    memset(&i2c_master_st, 0, sizeof i2c_master_st);
    memset(&crf, 0, sizeof crf);
    memset(&i2c_master_events_apst, 0, sizeof i2c_master_events_apst);
    memset(&pool_buff_au8, 0, sizeof pool_buff_au8);
    memset(&pool_ast, 0, sizeof pool_ast);
    memset(&tmp468_st, 0, sizeof tmp468_st);

    cpool_init(pool_ast + 0, pool_buff_au8, 24, 16);

    cqueue_init(&q_st, events_apst, 4);

    ut_i2c_driver_mock_init(&drv_mock_st);

    i2c_master_st.config_st.driver_pst = drv_pst;
    chsm_ctor(&i2c_master_st.super, i2c_master_top, i2c_master_events_apst, 4, 4);
    chsm_ctor(&tmp468_st.super, tmp468_top, tmp468_events_apst, 8, 0);

    tmp468_st.config_st = (tmp468_cfg_tst) { .address_u8 = 0x12, .id_u16 = 0xabcd, .temp_period_ms_u16 = 100, .max_error_cnt_u16 = 5 };

    CRF_SEND_FUNC(&tmp468_st) = tmp468_send;

    crf_init(&crf, hsm_apst, pool_ast, 1);
    chsm_init((chsm_tst *)&i2c_master_st);
    chsm_init((chsm_tst *)&tmp468_st);
}

TEST_TEAR_DOWN(tmp468)
{
}

/*
 * Just call setup
 */
TEST(tmp468, init)
{
    tick_ms(10);
}

TEST(tmp468, set_state_id_read_state)
{
    i2c_mock_slave_device_tst dev_st = { .address_u8 = TMP468_ADDR_SDA, .nack_idx_u16 = 20, .tx_data_au8 = { 0x01, 0x90, 0xf3, 0x80, 0x0c, 0x80 } };

    const tmp468_temp_tst *  e_pst;
    const tmp468_status_tst *s_pst;

    drv_mock_st.slave_pst = &dev_st;

    tick_ms(10);
    // tmp468_set_state(dev_st, e_pst, S_READ_TEMP);
    TEST_ASSERT_EQUAL(1, 0);
}

/* read_temp_twice:
 * Setup the mock device to send the id bytes and two temperature reading (-25 and 25) to the
 * master. Check, that two temperature events were sent
 */
TEST(tmp468, read_temp_twice)
{
    i2c_mock_slave_device_tst dev_st = { .address_u8 = TMP468_ADDR_SDA, .nack_idx_u16 = 20, .tx_data_au8 = { 0x01, 0x90, 0xf3, 0x80, 0x0c, 0x80 } };

    const tmp468_temp_tst *  e_pst;
    const tmp468_status_tst *s_pst;

    drv_mock_st.slave_pst = &dev_st;

    tick_ms(100);

    s_pst = (tmp468_status_tst *)q_st.get(&q_st);
    TEST_ASSERT(s_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_ONLINE, s_pst->super.sig);

    e_pst = (tmp468_temp_tst *)q_st.get(&q_st);
    TEST_ASSERT(e_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_TEMP, e_pst->super.sig);
    TEST_ASSERT_EQUAL(-25, e_pst->temp_C_i16);

    tick_ms(TMP468_READ_TEMP_PERIOD + 1);

    uint8_t expected_au8[4] = { 7, 0, 0, 0 };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_au8, dev_st.rx_data_au8, 4);
    TEST_ASSERT_EQUAL(2, dev_st.rx_idx_u16);
    TEST_ASSERT_EQUAL(6, dev_st.tx_idx_u16);

    e_pst = (tmp468_temp_tst *)q_st.get(&q_st);
    TEST_ASSERT(e_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_TEMP, e_pst->super.sig);
    TEST_ASSERT_EQUAL(25, e_pst->temp_C_i16);
}

/* read_id_retry:
 * Check that a failed ID read will be repeated after the configured time.
 */
TEST(tmp468, read_id_retry)
{
    i2c_mock_slave_device_tst dev_st = { .address_u8 = 0x13, .nack_idx_u16 = 20, .tx_data_au8 = { 0x01, 0x90, 0xf3, 0x80, 0x0c, 0x80 } };

    const tmp468_status_tst *s_pst;

    drv_mock_st.slave_pst = &dev_st;

    tick_ms(10);

    s_pst = (tmp468_status_tst *)q_st.get(&q_st);
    TEST_ASSERT_NULL(s_pst);

    dev_st.address_u8 = 0x12;

    tick_ms(TMP468_RETRY_TIMEOUT - 10);

    s_pst = (tmp468_status_tst *)q_st.get(&q_st);
    TEST_ASSERT_NULL(s_pst);

    tick_ms(10);

    s_pst = (tmp468_status_tst *)q_st.get(&q_st);
    TEST_ASSERT_NOT_NULL(s_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_ONLINE, s_pst->super.sig);
}

/* read_id_retry_bad_id:
 * Check that a failed ID read will be repeated after the configured time if the data sent back
 * by the slave is not 0x190 for the first try.
 */
TEST(tmp468, read_id_retry_bad_id)
{
    i2c_mock_slave_device_tst dev_st = { .address_u8 = 0x12, .nack_idx_u16 = 20, .tx_data_au8 = { 0x01, 0x91, 0x01, 0x90, 0x0c, 0x80 } };

    const tmp468_status_tst *s_pst;

    drv_mock_st.slave_pst = &dev_st;

    tick_ms(10);

    s_pst = (tmp468_status_tst *)q_st.get(&q_st);
    TEST_ASSERT_NULL(s_pst);

    tick_ms(TMP468_RETRY_TIMEOUT - 10);

    s_pst = (tmp468_status_tst *)q_st.get(&q_st);
    TEST_ASSERT_NULL(s_pst);

    tick_ms(10);

    s_pst = (tmp468_status_tst *)q_st.get(&q_st);
    TEST_ASSERT_NOT_NULL(s_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_ONLINE, s_pst->super.sig);
}

/* go_offline:
 * Check that after reading temperature data fails for more than TMP468_MAX_ERROR_COUNT times
 * the state machine goes offline and invalidates the temperature data.
 */
TEST(tmp468, go_offline)
{
    i2c_mock_slave_device_tst dev_st = { .address_u8 = 0x12, .nack_idx_u16 = 20, .tx_data_au8 = { 0x01, 0x90, 0xf3, 0x80, 0x0c, 0x80 } };

    const tmp468_temp_tst *  e_pst;
    const tmp468_status_tst *s_pst;

    drv_mock_st.slave_pst = &dev_st;

    tick_ms(10);

    s_pst = (tmp468_status_tst *)q_st.get(&q_st);
    TEST_ASSERT(s_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_ONLINE, s_pst->super.sig);

    e_pst = (tmp468_temp_tst *)q_st.get(&q_st);
    TEST_ASSERT(e_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_TEMP, e_pst->super.sig);
    TEST_ASSERT_EQUAL(-25, e_pst->temp_C_i16);

    tick_ms(TMP468_READ_TEMP_PERIOD + 1);

    e_pst = (tmp468_temp_tst *)q_st.get(&q_st);
    TEST_ASSERT(e_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_TEMP, e_pst->super.sig);
    TEST_ASSERT_EQUAL(25, e_pst->temp_C_i16);

    TEST_ASSERT_TRUE(tmp468_st.valid_b);

    dev_st.address_u8 = 0x13;

    tick_ms((TMP468_READ_TEMP_PERIOD + 1) * TMP468_MAX_ERROR_COUNT + 1);

    s_pst = (tmp468_status_tst *)q_st.get(&q_st);
    TEST_ASSERT(s_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_OFFLINE, s_pst->super.sig);

    TEST_ASSERT_FALSE(tmp468_st.valid_b);
}

/* triggered_read:
 * Check that it is possible to trigger a read before the read period timeout elapses.
 */
TEST(tmp468, triggered_read)
{
    i2c_mock_slave_device_tst dev_st = { .address_u8 = 0x12, .nack_idx_u16 = 20, .tx_data_au8 = { 0x01, 0x90, 0xf3, 0x80, 0x0c, 0x80 } };

    const cevent_tst read_event = { .sig = SIG_TMP468_TEMP };

    const tmp468_temp_tst *  e_pst;
    const tmp468_status_tst *s_pst;

    drv_mock_st.slave_pst = &dev_st;

    tick_ms(10);

    s_pst = (tmp468_status_tst *)q_st.get(&q_st);
    TEST_ASSERT(s_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_ONLINE, s_pst->super.sig);

    e_pst = (tmp468_temp_tst *)q_st.get(&q_st);
    TEST_ASSERT(e_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_TEMP, e_pst->super.sig);
    TEST_ASSERT_EQUAL(-25, e_pst->temp_C_i16);

    tick_ms(1);

    CRF_POST(&read_event, &tmp468_st);

    tick_ms(10);

    uint8_t expected_au8[4] = { 7, 0, 0, 0 };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_au8, dev_st.rx_data_au8, 4);
    TEST_ASSERT_EQUAL(2, dev_st.rx_idx_u16);
    TEST_ASSERT_EQUAL(6, dev_st.tx_idx_u16);

    e_pst = (tmp468_temp_tst *)q_st.get(&q_st);
    TEST_ASSERT(e_pst);
    TEST_ASSERT_EQUAL(SIG_TMP468_TEMP, e_pst->super.sig);
    TEST_ASSERT_EQUAL(25, e_pst->temp_C_i16);
}

TEST_GROUP_RUNNER(tmp468)
{
    RUN_TEST_CASE(tmp468, init);
    // RUN_TEST_CASE(tmp468, set_state_id_read_state);
    // RUN_TEST_CASE(tmp468, read_temp_twice);
    // RUN_TEST_CASE(tmp468, read_id_retry);
    // RUN_TEST_CASE(tmp468, read_id_retry_bad_id);
    // RUN_TEST_CASE(tmp468, go_offline);
    // RUN_TEST_CASE(tmp468, triggered_read);
}
