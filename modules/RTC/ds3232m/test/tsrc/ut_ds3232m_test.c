
    /*
 * ut_ds3232m_test.c
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
#include "ds3232m.h"
#include "cevent.h"
#include "sys_if.h"

TEST_GROUP(ds3232m);

const cevent_tst*		ds3232m_events_apst[8];
ds3232m_tst				ds3232m_st;


ds3232m_tst *self = 		&ds3232m_st; 

chsm_tst*				hsm_apst[] = {
	(chsm_tst*)(&ds3232m_st),
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

		CRF_POST(&tick_1ms_st, &ds3232m_st);

		while(CRF_STEP())
		{
			printf("|");
		}
	}
}

void ds3232m_send(chsm_tst *self, const cevent_tst *e_pst)
{
	switch(e_pst->sig)
	{
		case SIG_DS3232M_ONLINE:
		case SIG_DS3232M_OFFLINE:
			CRF_POST(e_pst, &q_st);
			break;

		default:
	}
}


TEST_SETUP(ds3232m)
{
    memset(events_apst, 0, sizeof events_apst);
    memset(&q_st, 0, sizeof q_st);
    memset(&crf, 0, sizeof crf);
    memset(&pool_buff_au8, 0, sizeof pool_buff_au8);
    memset(&pool_ast, 0, sizeof pool_ast);
    memset(&ds3232m_st, 0, sizeof ds3232m_st);

	cpool_init(pool_ast+0, pool_buff_au8, 24, 16);
    cqueue_init(&q_st, events_apst, 4);
	chsm_ctor(&ds3232m_st.super, ds3232m_top, ds3232m_events_apst, 8, 0);
	

	CRF_SEND_FUNC(&ds3232m_st) = ds3232m_send;

	crf_init(&crf, hsm_apst, pool_ast, 1);
	chsm_init((chsm_tst *)&ds3232m_st);
}

TEST_TEAR_DOWN(ds3232m)
{
}

/*
 * Just call setup
 */
TEST(ds3232m, init)
{
}

TEST_GROUP_RUNNER(ds3232m)
{
	RUN_TEST_CASE(ds3232m, init);
}
    
    