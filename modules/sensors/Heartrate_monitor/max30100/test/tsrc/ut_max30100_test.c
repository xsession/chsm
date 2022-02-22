
    /*
 * ut_max30100_test.c
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "unity_fixture.h"
#include "crf.h"
#include "cbits.h"
#include "max30100.h"
#include "cevent.h"
#include "sys_if.h"

TEST_GROUP(max30100);

const cevent_tst*		max30100_events_apst[8];
max30100_tst				max30100_st;


max30100_tst *self = 		&max30100_st; 

chsm_tst*				hsm_apst[] = {
	(chsm_tst*)(&max30100_st),
	NULL};

uint8_t 				pool_buff_au8[1024];
cpool_tst				pool_ast[1];

crf_tst					crf;

const cevent_tst*       events_apst[4];
cqueue_tst              q_st;

const cevent_tst		tick_1ms_st = {.sig = SIG_SYS_TICK_1ms};

static void tick_ms(uint32_t tick_cnt_u32)
{
	while(tick_cnt_u32--)
	{
	    // drv_mock_st.tick(&drv_mock_st);

		CRF_POST(&tick_1ms_st, &max30100_st);

		while(CRF_STEP())
		{
			printf("|");
		}
	}
}

void max30100_send(chsm_tst *self, const cevent_tst *e_pst)
{
	switch(e_pst->sig)
	{
		case SIG_MAX30100_ONLINE:
		case SIG_MAX30100_OFFLINE:
			CRF_POST(e_pst, &q_st);
			break;

		default:
	}
}


TEST_SETUP(max30100)
{
    memset(events_apst, 0, sizeof events_apst);
    memset(&q_st, 0, sizeof q_st);
    memset(&crf, 0, sizeof crf);
    memset(&pool_buff_au8, 0, sizeof pool_buff_au8);
    memset(&pool_ast, 0, sizeof pool_ast);
    memset(&max30100_st, 0, sizeof max30100_st);

	cpool_init(pool_ast+0, pool_buff_au8, 24, 16);
    cqueue_init(&q_st, events_apst, 4);
	chsm_ctor(&max30100_st.super, max30100_top, max30100_events_apst, 8, 0);
	

	CRF_SEND_FUNC(&max30100_st) = max30100_send;

	crf_init(&crf, hsm_apst, pool_ast, 1);
	chsm_init((chsm_tst *)&max30100_st);
}

TEST_TEAR_DOWN(max30100)
{
}

/*
 * Just call setup
 */
TEST(max30100, init)
{
}

TEST_GROUP_RUNNER(max30100)
{
	RUN_TEST_CASE(max30100, init);
}
    
    