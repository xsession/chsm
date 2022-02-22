
    /*
 * ut_apds9960_test.c
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
#include "apds9960.h"
#include "cevent.h"
#include "sys_if.h"

TEST_GROUP(apds9960);

const cevent_tst*		apds9960_events_apst[8];
apds9960_tst				apds9960_st;


apds9960_tst *self = 		&apds9960_st; 

chsm_tst*				hsm_apst[] = {
	(chsm_tst*)(&apds9960_st),
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

		CRF_POST(&tick_1ms_st, &apds9960_st);

		while(CRF_STEP())
		{
			printf("|");
		}
	}
}

void apds9960_send(chsm_tst *self, const cevent_tst *e_pst)
{
	switch(e_pst->sig)
	{
		case SIG_APDS9960_ONLINE:
		case SIG_APDS9960_OFFLINE:
			CRF_POST(e_pst, &q_st);
			break;

		default:
	}
}


TEST_SETUP(apds9960)
{
    memset(events_apst, 0, sizeof events_apst);
    memset(&q_st, 0, sizeof q_st);
    memset(&crf, 0, sizeof crf);
    memset(&pool_buff_au8, 0, sizeof pool_buff_au8);
    memset(&pool_ast, 0, sizeof pool_ast);
    memset(&apds9960_st, 0, sizeof apds9960_st);

	cpool_init(pool_ast+0, pool_buff_au8, 24, 16);
    cqueue_init(&q_st, events_apst, 4);
	chsm_ctor(&apds9960_st.super, apds9960_top, apds9960_events_apst, 8, 0);
	

	CRF_SEND_FUNC(&apds9960_st) = apds9960_send;

	crf_init(&crf, hsm_apst, pool_ast, 1);
	chsm_init((chsm_tst *)&apds9960_st);
}

TEST_TEAR_DOWN(apds9960)
{
}

/*
 * Just call setup
 */
TEST(apds9960, init)
{
}

TEST_GROUP_RUNNER(apds9960)
{
	RUN_TEST_CASE(apds9960, init);
}
    
    