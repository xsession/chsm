
    /*
 * ut_mpr121_test.c
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
#include "mpr121.h"
#include "cevent.h"
#include "sys_if.h"

TEST_GROUP(mpr121);

const cevent_tst*		mpr121_events_apst[8];
mpr121_tst				mpr121_st;


mpr121_tst *self = 		&mpr121_st; 

chsm_tst*				hsm_apst[] = {
	(chsm_tst*)(&mpr121_st),
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

		CRF_POST(&tick_1ms_st, &mpr121_st);

		while(CRF_STEP())
		{
			printf("|");
		}
	}
}

void mpr121_send(chsm_tst *self, const cevent_tst *e_pst)
{
	switch(e_pst->sig)
	{
		case SIG_MPR121_ONLINE:
		case SIG_MPR121_OFFLINE:
			CRF_POST(e_pst, &q_st);
			break;

		default:
	}
}


TEST_SETUP(mpr121)
{
    memset(events_apst, 0, sizeof events_apst);
    memset(&q_st, 0, sizeof q_st);
    memset(&crf, 0, sizeof crf);
    memset(&pool_buff_au8, 0, sizeof pool_buff_au8);
    memset(&pool_ast, 0, sizeof pool_ast);
    memset(&mpr121_st, 0, sizeof mpr121_st);

	cpool_init(pool_ast+0, pool_buff_au8, 24, 16);
    cqueue_init(&q_st, events_apst, 4);
	chsm_ctor(&mpr121_st.super, mpr121_top, mpr121_events_apst, 8, 0);
	

	CRF_SEND_FUNC(&mpr121_st) = mpr121_send;

	crf_init(&crf, hsm_apst, pool_ast, 1);
	chsm_init((chsm_tst *)&mpr121_st);
}

TEST_TEAR_DOWN(mpr121)
{
}

/*
 * Just call setup
 */
TEST(mpr121, init)
{
}

TEST_GROUP_RUNNER(mpr121)
{
	RUN_TEST_CASE(mpr121, init);
}
    
    