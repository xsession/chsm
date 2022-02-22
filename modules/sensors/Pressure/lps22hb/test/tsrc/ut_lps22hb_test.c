
    /*
 * ut_lps22hb_test.c
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
#include "lps22hb.h"
#include "cevent.h"
#include "sys_if.h"

TEST_GROUP(lps22hb);

const cevent_tst*		lps22hb_events_apst[8];
lps22hb_tst				lps22hb_st;


lps22hb_tst *self = 		&lps22hb_st; 

chsm_tst*				hsm_apst[] = {
	(chsm_tst*)(&lps22hb_st),
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

		CRF_POST(&tick_1ms_st, &lps22hb_st);

		while(CRF_STEP())
		{
			printf("|");
		}
	}
}

void lps22hb_send(chsm_tst *self, const cevent_tst *e_pst)
{
	switch(e_pst->sig)
	{
		case SIG_LPS22HB_ONLINE:
		case SIG_LPS22HB_OFFLINE:
			CRF_POST(e_pst, &q_st);
			break;

		default:
	}
}


TEST_SETUP(lps22hb)
{
    memset(events_apst, 0, sizeof events_apst);
    memset(&q_st, 0, sizeof q_st);
    memset(&crf, 0, sizeof crf);
    memset(&pool_buff_au8, 0, sizeof pool_buff_au8);
    memset(&pool_ast, 0, sizeof pool_ast);
    memset(&lps22hb_st, 0, sizeof lps22hb_st);

	cpool_init(pool_ast+0, pool_buff_au8, 24, 16);
    cqueue_init(&q_st, events_apst, 4);
	chsm_ctor(&lps22hb_st.super, lps22hb_top, lps22hb_events_apst, 8, 0);
	

	CRF_SEND_FUNC(&lps22hb_st) = lps22hb_send;

	crf_init(&crf, hsm_apst, pool_ast, 1);
	chsm_init((chsm_tst *)&lps22hb_st);
}

TEST_TEAR_DOWN(lps22hb)
{
}

/*
 * Just call setup
 */
TEST(lps22hb, init)
{
}

TEST_GROUP_RUNNER(lps22hb)
{
	RUN_TEST_CASE(lps22hb, init);
}
    
    