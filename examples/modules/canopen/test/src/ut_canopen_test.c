/*
 * ut_fram_test.c
 *
 *  Created on: 2020. dec. 30.
 *      Author: jszeman
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "unity_fixture.h"
#include "crf.h"
#include "cevent.h"
#include "canopen.h"
#include "can_if.h"

TEST_GROUP(co);

const cevent_tst*		node_events_apst[12];
co_node_tst				node_st;


co_node_tst *self = 	&node_st;

chsm_tst*				hsm_apst[] = {
	(chsm_tst*)(&node_st),
	NULL};

uint8_t 				pool_buff_au8[1024];
cpool_tst				pool_ast[1];

crf_tst					crf;

const cevent_tst*       events_apst[4];
cqueue_tst              q_st;

void*					reset_param_pv;

uint8_t					obj_u8;
uint16_t				obj_u16;
uint32_t				obj_u32;
char					str_ac[32] = "String object";

od_entry_tst			od_entries_ast[] = {
	OD_ENTRY_DEF(0x123400, obj_u8, OD_ATTR_READ | OD_ATTR_WRITE),
	OD_ENTRY_DEF(0x123500, obj_u16, OD_ATTR_READ | OD_ATTR_WRITE),
	OD_ENTRY_DEF(0x123600, obj_u32, OD_ATTR_READ | OD_ATTR_WRITE),
	OD_ENTRY_DEF(0x123700, str_ac, OD_ATTR_READ | OD_ATTR_WRITE),
	OD_ENTRY_TERMINATOR,
};

object_dictionary_tst	od_st = {
	.mem_drv_pst = 		NULL,
	.mlx_mask_u32 = 	0xffffffff,
	.objects_ast = 		od_entries_ast
};

void on_nmt_reset(void *pv)
{
	reset_param_pv = pv;
}

void co_send(chsm_tst *self, const cevent_tst *e_pst)
{
    //printf("\n%s\n", __FUNCTION__);
	switch(e_pst->sig)
	{
		default:
			CRF_POST(e_pst, &q_st);
	}
}


const cevent_tst		tick_1ms_st = {.sig = SIG_SYS_TICK_1ms};

static void tick_ms(uint32_t tick_cnt_u32)
{
	while(tick_cnt_u32--)
	{
		CRF_POST(&tick_1ms_st, &node_st);

		while(CRF_STEP())
		{
			printf("|");
		}
	}
}

void node_init(void)
{
	const can_frame_tst *e_pst;

	chsm_init((chsm_tst *)&node_st);
	e_pst = (can_frame_tst *)q_st.get(&q_st);
	CRF_GC(e_pst);
}

TEST_SETUP(co)
{
	obj_u8 = 0x89;
	obj_u16 = 0xbb66;
	obj_u32 = 0xaa55aa55;
	strcpy(str_ac, "String object");

	reset_param_pv = NULL;

    memset(&node_events_apst, 0, sizeof node_events_apst);
    memset(&node_st, 0, sizeof node_st);
	
	chsm_ctor(&node_st.super, co_node_top, node_events_apst, 8, 4);
	
	node_st.config_st = (co_node_cfg_tst){
		.node_id_u8 = 0x11,
		.od_pst = &od_st,
		.user_param_pv = on_nmt_reset,
		.on_nmt_reset = on_nmt_reset,
		};

	node_st.super.send = co_send;
}

TEST_TEAR_DOWN(co)
{
}

/* init
 * Init the CRF. It is only done once, but that should not be a problem,
 * as long as all resources are properly garbage collected. So in a way,
 * this is also a test for the framework.
 */
TEST(co, init)
{
    memset(&pool_buff_au8, 0, sizeof pool_buff_au8);
    memset(&pool_ast, 0, sizeof pool_ast);
    memset(events_apst, 0, sizeof events_apst);
    memset(&q_st, 0, sizeof q_st);
    cqueue_init(&q_st, events_apst, 4);
    memset(&crf, 0, sizeof crf);
	cpool_init(pool_ast+0, pool_buff_au8, 24, 16);
	crf_init(&crf, hsm_apst, pool_ast, 1);
}

/* bootup
 * Check that the bootup message is sent after init.
 */
TEST(co, bootup)
{
	const can_frame_tst *e_pst;

	chsm_init((chsm_tst *)&node_st);

	e_pst = (const can_frame_tst *)q_st.get(&q_st);

	TEST_ASSERT_NOT_NULL(e_pst)
	TEST_ASSERT_EQUAL(SIG_CAN_FRAME, e_pst->super.sig);
	
	TEST_ASSERT_EQUAL_HEX(node_st.config_st.node_id_u8 + CO_NMT, e_pst->header_un.bit_st.id_u12);
	TEST_ASSERT_EQUAL(1, e_pst->header_un.bit_st.dlc_u4);
	TEST_ASSERT_EQUAL(0, e_pst->header_un.bit_st.rtr_u1);
	TEST_ASSERT_EQUAL_HEX(0, e_pst->mdl_un.bit_st.d0_u8);

	CRF_GC(e_pst);
}

/* nodeguard_req
 * Check that the node responds to a nodeguard request
 */
TEST(co, nodeguard_req)
{
	const can_frame_tst *e_pst;
	can_frame_tst *ng_pst;

	node_init();

	ng_pst = CRF_NEW(SIG_CAN_FRAME);
	TEST_ASSERT_NOT_NULL(ng_pst)

	ng_pst->header_un = CAN_HDR(CO_NMT + self->config_st.node_id_u8, 1, 0);
	CRF_POST(ng_pst, &node_st);

	tick_ms(1);

	e_pst = (const can_frame_tst *)q_st.get(&q_st);

	TEST_ASSERT_NOT_NULL(e_pst)
	TEST_ASSERT_EQUAL(SIG_CAN_FRAME, e_pst->super.sig);
	
	TEST_ASSERT_EQUAL_HEX(node_st.config_st.node_id_u8 + CO_NMT, e_pst->header_un.bit_st.id_u12);
	TEST_ASSERT_EQUAL(1, e_pst->header_un.bit_st.dlc_u4);
	TEST_ASSERT_EQUAL(0, e_pst->header_un.bit_st.rtr_u1);
	TEST_ASSERT_EQUAL_HEX(0x7f, e_pst->mdl_un.bit_st.d0_u8);
	CRF_GC(e_pst);
}

/* nodeguard_toggle
 * Check that the node responds to a nodeguard request and toggles the
 * MSB in consecutive responses
 */
TEST(co, nodeguard_toggle)
{
	const can_frame_tst *e_pst;
	can_frame_tst *ng_pst;

	node_init();

	for (int i=0; i<10; i++)
	{
		ng_pst = CRF_NEW(SIG_CAN_FRAME);
		TEST_ASSERT_NOT_NULL(ng_pst)

		ng_pst->header_un = CAN_HDR(CO_NMT + self->config_st.node_id_u8, 1, 0);
		CRF_POST(ng_pst, &node_st);

		tick_ms(1);

		e_pst = (const can_frame_tst *)q_st.get(&q_st);

		TEST_ASSERT_NOT_NULL(e_pst)
		TEST_ASSERT_EQUAL(SIG_CAN_FRAME, e_pst->super.sig);
		
		TEST_ASSERT_EQUAL_HEX(node_st.config_st.node_id_u8 + CO_NMT, e_pst->header_un.bit_st.id_u12);
		TEST_ASSERT_EQUAL(1, e_pst->header_un.bit_st.dlc_u4);
		TEST_ASSERT_EQUAL(0, e_pst->header_un.bit_st.rtr_u1);
		TEST_ASSERT_EQUAL_HEX(0x7f | (i & 1 ? 0x80 : 0), e_pst->mdl_un.bit_st.d0_u8);
		CRF_GC(e_pst);
	}
}

/* nmt_reset
 * Check that on_nmt_reset function is called when an NMT reset frame is received
 */
TEST(co, nmt_reset)
{
	const can_frame_tst *e_pst;
	can_frame_tst *f_pst;

	node_init();

	f_pst = CRF_NEW(SIG_CAN_FRAME);
	TEST_ASSERT_NOT_NULL(f_pst)

	f_pst->header_un = CAN_HDR(0, 0, 2);
	f_pst->mdl_un.bit_st.d0_u8 = CO_NMT_CMD_RESET;
	f_pst->mdl_un.bit_st.d1_u8 = node_st.config_st.node_id_u8;
	CRF_POST(f_pst, &node_st);

	tick_ms(1);

	TEST_ASSERT_EQUAL_PTR(on_nmt_reset, reset_param_pv);
}



/* nmt_start
 * Check that a START command changes the code in NG responses
 */
TEST(co, nmt_start)
{
	const can_frame_tst *e_pst;
	can_frame_tst *f_pst;
	can_frame_tst *ng_pst;

	node_init();

	f_pst = CRF_NEW(SIG_CAN_FRAME);
	TEST_ASSERT_NOT_NULL(f_pst)

	f_pst->header_un = CAN_HDR(0, 0, 2);
	f_pst->mdl_un.bit_st.d0_u8 = CO_NMT_CMD_START;
	f_pst->mdl_un.bit_st.d1_u8 = node_st.config_st.node_id_u8;
	CRF_POST(f_pst, &node_st);

	tick_ms(1);

	for (int i=0; i<10; i++)
	{
		ng_pst = CRF_NEW(SIG_CAN_FRAME);
		TEST_ASSERT_NOT_NULL(ng_pst)

		ng_pst->header_un = CAN_HDR(CO_NMT + self->config_st.node_id_u8, 1, 0);
		CRF_POST(ng_pst, &node_st);

		tick_ms(1);

		e_pst = (const can_frame_tst *)q_st.get(&q_st);

		TEST_ASSERT_NOT_NULL(e_pst)
		TEST_ASSERT_EQUAL(SIG_CAN_FRAME, e_pst->super.sig);
		
		TEST_ASSERT_EQUAL_HEX(node_st.config_st.node_id_u8 + CO_NMT, e_pst->header_un.bit_st.id_u12);
		TEST_ASSERT_EQUAL(1, e_pst->header_un.bit_st.dlc_u4);
		TEST_ASSERT_EQUAL(0, e_pst->header_un.bit_st.rtr_u1);
		TEST_ASSERT_EQUAL_HEX(0x05 | (i & 1 ? 0x80 : 0), e_pst->mdl_un.bit_st.d0_u8);
		CRF_GC(e_pst);
	}
}



/* nmt_preop
 * Check that a PREOP command changes the code in NG responses
 */
TEST(co, nmt_preop)
{
	const can_frame_tst *e_pst;
	can_frame_tst *f_pst;
	can_frame_tst *ng_pst;

	node_init();

	/* Go to STARTED state */
	f_pst = CRF_NEW(SIG_CAN_FRAME);
	TEST_ASSERT_NOT_NULL(f_pst)

	f_pst->header_un = CAN_HDR(0, 0, 2);
	f_pst->mdl_un.bit_st.d0_u8 = CO_NMT_CMD_START;
	f_pst->mdl_un.bit_st.d1_u8 = node_st.config_st.node_id_u8;
	CRF_POST(f_pst, &node_st);

	tick_ms(1);

	/* Go to PREOP state */
	f_pst = CRF_NEW(SIG_CAN_FRAME);
	TEST_ASSERT_NOT_NULL(f_pst)

	f_pst->header_un = CAN_HDR(0, 0, 2);
	f_pst->mdl_un.bit_st.d0_u8 = CO_NMT_CMD_PREOP;
	f_pst->mdl_un.bit_st.d1_u8 = node_st.config_st.node_id_u8;
	CRF_POST(f_pst, &node_st);

	tick_ms(1);

	for (int i=0; i<10; i++)
	{
		ng_pst = CRF_NEW(SIG_CAN_FRAME);
		TEST_ASSERT_NOT_NULL(ng_pst)

		ng_pst->header_un = CAN_HDR(CO_NMT + self->config_st.node_id_u8, 1, 0);
		CRF_POST(ng_pst, &node_st);

		tick_ms(1);

		e_pst = (const can_frame_tst *)q_st.get(&q_st);

		TEST_ASSERT_NOT_NULL(e_pst)
		TEST_ASSERT_EQUAL(SIG_CAN_FRAME, e_pst->super.sig);
		
		TEST_ASSERT_EQUAL_HEX(node_st.config_st.node_id_u8 + CO_NMT, e_pst->header_un.bit_st.id_u12);
		TEST_ASSERT_EQUAL(1, e_pst->header_un.bit_st.dlc_u4);
		TEST_ASSERT_EQUAL(0, e_pst->header_un.bit_st.rtr_u1);
		TEST_ASSERT_EQUAL_HEX(CO_NMT_STATE_PREOP | (i & 1 ? 0x80 : 0), e_pst->mdl_un.bit_st.d0_u8);
		CRF_GC(e_pst);
	}
}



/* sdo_dl_exp_1b
 * Initiate an expedited 1 byte download and check that the value was correctly written.
 */
TEST(co, sdo_dl_exp_1b)
{
	const can_frame_tst *e_pst;
	can_frame_tst *f_pst;

	node_init();

	f_pst = CRF_NEW(SIG_CAN_FRAME);
	TEST_ASSERT_NOT_NULL(f_pst);

	f_pst->header_un = CAN_HDR(CO_SDO_RX + node_st.config_st.node_id_u8, 0, 8);
	f_pst->mdl_un.bit_st.d0_u8 = CO_SDO_DL_REQ_EXP_1B;
	f_pst->mdl_un.bit_st.d1_u8 = 0x34;
	f_pst->mdl_un.bit_st.d2_u8 = 0x12;
	f_pst->mdl_un.bit_st.d3_u8 = 0x00;
	f_pst->mdh_un.bit_st.d4_u8 = 0xa5;
	CRF_POST(f_pst, &node_st);

	tick_ms(1);

	e_pst = (const can_frame_tst *)q_st.get(&q_st);

	TEST_ASSERT_NOT_NULL(e_pst)
	TEST_ASSERT_EQUAL(SIG_CAN_FRAME, e_pst->super.sig);
	
	TEST_ASSERT_EQUAL_HEX(node_st.config_st.node_id_u8 + CO_SDO_TX, e_pst->header_un.bit_st.id_u12);
	TEST_ASSERT_EQUAL(8, e_pst->header_un.bit_st.dlc_u4);
	TEST_ASSERT_EQUAL(0, e_pst->header_un.bit_st.rtr_u1);
	CRF_GC(e_pst);
}

TEST_GROUP_RUNNER(co)
{
	RUN_TEST_CASE(co, init);
	RUN_TEST_CASE(co, bootup);
	RUN_TEST_CASE(co, nodeguard_req);
	RUN_TEST_CASE(co, nodeguard_toggle);
	RUN_TEST_CASE(co, nmt_reset);
	RUN_TEST_CASE(co, nmt_start);
	RUN_TEST_CASE(co, nmt_preop);
	RUN_TEST_CASE(co, sdo_dl_exp_1b);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);
	//RUN_TEST_CASE(co, init);

}
