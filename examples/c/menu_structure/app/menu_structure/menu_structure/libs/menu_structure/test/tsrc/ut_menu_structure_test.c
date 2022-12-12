/*
 * ut_menu_structure_test.c
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
#include "cbits.h"
#include "menu_structure.h"
#include "cevent.h"
#include "sys_if.h"

TEST_GROUP(menu_structure);

const cevent_tst*		menu_structure_events_apst[8];
menu_structure_tst				menu_structure_st;
menu_structure_tst *self = 		&menu_structure_st; 

chsm_tst*				hsm_apst[] = {
	(chsm_tst*)(&menu_structure_st),
	NULL};

uint8_t 				pool_buff_au8[128];
cpool_tst				pool_ast[1];

crf_tst					crf;

const cevent_tst*       events_apst[4];
cqueue_tst              q_st;

const cevent_tst		tick_10ms_st = {.sig = SIG_SYS_TICK_10ms};

static void tick_ms(uint32_t tick_cnt_u32)
{
	while(tick_cnt_u32--)
	{
		drv_mock_st.tick(&drv_mock_st);

		CRF_POST(&tick_10ms_st, &menu_structure_st);

		while(CRF_STEP())
		{
			printf("|");
		}
	}
}

void menu_structure_send(chsm_tst *self, const cevent_tst *event_pst)
{
	switch(event_pst->sig)
	{
		case SIG_MENU_STRUCTURE_TEMP:
			CRF_POST(event_pst, &q_st);
			break;

		default:
			CRF_POST(event_pst, &menu_structure_st);
	}
}


TEST_SETUP(menu_structure)
{
    memset(events_apst, 0, sizeof events_apst);
    memset(&q_st, 0, sizeof q_st);
    memset(&crf, 0, sizeof crf);
    memset(&pool_buff_au8, 0, sizeof pool_buff_au8);
    memset(&pool_ast, 0, sizeof pool_ast);
    memset(&menu_structure_st, 0, sizeof menu_structure_st);

	cpool_init(pool_ast+0, pool_buff_au8, 24, 16);
    cqueue_init(&q_st, events_apst, 4);
	chsm_ctor(&menu_structure_st.super, menu_structure_top, menu_structure_events_apst, 8, 0);
	
	CRF_SEND_FUNC(&menu_structure_st) = menu_structure_send;

	crf_init(&crf, hsm_apst, pool_ast, 1);
	chsm_init((chsm_tst *)&menu_structure_st);
}

TEST_TEAR_DOWN(menu_structure)
{
}

/*
 * Just call setup
 */
TEST(menu_structure, init)
{
}

/* read_temp_twice:
 */
TEST(menu_structure, read_temp_twice)
{
	const menu_structure_tst* event_pst;

	status_pst = (menu_structure_status_tst*)q_st.get(&q_st);
	TEST_ASSERT(s_pst);
	TEST_ASSERT_EQUAL(SIG_MENU_STRUCTURE_ONLINE, s_pst->super.sig);

	e_pst = (menu_structure_temp_tst*)q_st.get(&q_st);
	TEST_ASSERT(e_pst);
	TEST_ASSERT_EQUAL(SIG_MENU_STRUCTURE_TEMP, e_pst->super.sig);
	TEST_ASSERT_EQUAL(-25, e_pst->temp_C_i32);
}

TEST_GROUP_RUNNER(menu_structure)
{
	RUN_TEST_CASE(menu_structure, init);
	RUN_TEST_CASE(menu_structure, read_temp_twice);
}
