#include <stdio.h>
#include <time.h>
#include "crf.h"
#include "chsm.h"
#include "signal_classes.h"
#include "menu_structure.h"

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
    (chsm_tst *) &menu_tst,
    NULL
};

const cevent_tst*   menu_structure_events_apst[8];

uint8_t             pool_buff_au8[1024];
cpool_tst           pool_ast[1];
crf_tst             crf;

const cevent_tst*       events_apst[4];
cqueue_tst              q_st;


static const cevent_tst one_transaction_st = {.sig = SIG_TRANS_1};
static const cevent_tst two_transaction_st = {.sig = SIG_TRANS_2};
static const cevent_tst three_transaction_st = {.sig = SIG_TRANS_3};

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