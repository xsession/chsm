#include <stdio.h>
#include <time.h>
#include "crf.h"
#include "chsm.h"
#include "signal_classes.h"
#include "chsm_demo.h"

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

chsm_tst *hsm_ap[] = {
    (chsm_tst *) &chsm_demo_tst,
    NULL
};

const cevent_tst*   chsm_demo_events_apst[8];

uint8_t             pool_buff_au8[1024];
cpool_tst           pool_ast[1];
crf_tst             crf;

const cevent_tst*       events_apst[4];
cqueue_tst              q_st;


static const cevent_tst event_none = {.sig=C_SIG_NONE, .gc_info=0};

const cevent_tst event1 = {.sig=TEST_SIG1, .gc_info=0};
const cevent_tst event2 = {.sig=TEST_SIG2, .gc_info=0};
const cevent_tst event3 = {.sig=TEST_SIG3, .gc_info=0};

const cevent_tst event_a = {.sig=TEST_SIG_A, .gc_info=0};
const cevent_tst event_b = {.sig=TEST_SIG_B, .gc_info=0};
const cevent_tst event_c = {.sig=TEST_SIG_C, .gc_info=0};
const cevent_tst event_d = {.sig=TEST_SIG_D, .gc_info=0};
const cevent_tst event_e = {.sig=TEST_SIG_E, .gc_info=0};
const cevent_tst event_f = {.sig=TEST_SIG_F, .gc_info=0};
const cevent_tst event_g = {.sig=TEST_SIG_G, .gc_info=0};
const cevent_tst event_h = {.sig=TEST_SIG_H, .gc_info=0};
const cevent_tst event_j = {.sig=TEST_SIG_J, .gc_info=0};
const cevent_tst event_k = {.sig=TEST_SIG_K, .gc_info=0};
const cevent_tst event_l = {.sig=TEST_SIG_L, .gc_info=0};
const cevent_tst event_m = {.sig=TEST_SIG_M, .gc_info=0};
const cevent_tst event_id = {.sig=TEST_SIG_ID, .gc_info=0};

uint8_t input[10] = {0};
uint16_t num = 0;

int main()
{
    cpool_init(pool_ast+0, pool_buff_au8, 32, 32);
    chsm_ctor(&menu_tst.super, menu_structure_top, menu_structure_events_apst, 8, 0);
    chsm_init((chsm_tst *)&menu_tst);
    crf_init(&crf, hsm_ap, pool_ast, 1);

        // CRF_POST(&five_transaction_st, &menu_tst);
    while(1)
    {
        fgets(input, 10, stdin);
        sscanf(input, "%d", &num);
        printf("The entered num: %d\n", num);  
        switch (num)
        {
        case 1:
            CRF_POST(&one_transaction_st, &menu_tst);
            break;
        case 2:
            CRF_POST(&two_transaction_st, &menu_tst);
            break;
        case 3:
            CRF_POST(&three_transaction_st, &menu_tst);
            break;
        
        default:
            break;
        } 
        CRF_STEP();
    }
    return 0;
}