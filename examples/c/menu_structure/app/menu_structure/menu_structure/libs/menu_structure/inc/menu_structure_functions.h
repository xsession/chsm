#ifndef MENU_STRUCTURE_FUNCTIONS_H
#define MENU_STRUCTURE_FUNCTIONS_H

/*Generated with CHSM v0.0.0 at 2021.05.17 10.36.44*/

#include "menu_structure.h"
#include "chsm.h"
#include "cevent.h"
#include <stdbool.h>


void exit_func(chsm_tst *self, const cevent_tst *e_pst);

void exit_inner_func(chsm_tst *self, const cevent_tst *e_pst);

void init_func(chsm_tst *self, const cevent_tst *e_pst);

void state1_func(chsm_tst *self, const cevent_tst *e_pst);

void state2_func(chsm_tst *self, const cevent_tst *e_pst);

void state_1_inner_state_func(chsm_tst *self, const cevent_tst *e_pst);

void state_3_func(chsm_tst *self, const cevent_tst *e_pst);


typedef enum menu_structure_state_id_ten
{
    MENU_STRUCTURE = 0,
    STATE_1 = 1,
    STATE_2 = 2,
    STATE_1_INNER_STATE = 3,
    STATE_3 = 4,
} menu_structure_state_id_ten;


/*
Signals:
    SIG_TRANS_1 
    SIG_TRANS_2 
    SIG_TRANS_3 
*/

/*
Other function notes:
*/
#endif
