#include <stdio.h>
#include "menu_structure_functions.h"
#include "menu_structure.h"


static const cevent_tst one_transaction_st = {.sig = SIG_TRANS_1};
static const cevent_tst two_transaction_st = {.sig = SIG_TRANS_2};
static const cevent_tst third_transaction_st = {.sig = SIG_TRANS_3};

void init_func(chsm_tst *_self, const cevent_tst *e_pst)
{
    printf("\n%s\n", __FUNCTION__);

    // menu_structure_tst*   _self =          (menu_structure_tst *)self;
    printf("Hello from init_func!");
}

void exit_func(chsm_tst *self, const cevent_tst *e_pst)
{
  printf("\n%s\n", __FUNCTION__);

}

void state1_func(chsm_tst *self, const cevent_tst *e_pst)
{
  printf("\n%s\n", __FUNCTION__);
    // CRF_POST_TO_SELF(&one_to_two_transaction_st);

}

void state2_func(chsm_tst *self, const cevent_tst *e_pst)
{
  printf("\n%s\n", __FUNCTION__);
  // CRF_POST_TO_SELF(&two_to_three_transaction_st);
}

void state_1_inner_state_func(chsm_tst *self, const cevent_tst *e_pst)
{
    printf("\n%s\n", __FUNCTION__);
  // CRF_POST_TO_SELF(&five_transaction_st);
}

void exit_inner_func(chsm_tst *self, const cevent_tst *e_pst)
{
    printf("\n%s\n", __FUNCTION__);
}

void state_3_func(chsm_tst *self, const cevent_tst *e_pst)
{
    printf("\n%s\n", __FUNCTION__);
}
