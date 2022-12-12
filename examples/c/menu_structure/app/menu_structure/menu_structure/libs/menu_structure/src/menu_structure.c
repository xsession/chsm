/*Generated with CHSM v0.0.0 at 2021.05.17 10.36.44*/
#include "cevent.h"
#include "chsm.h"
#include "menu_structure.h"
#include "menu_structure_functions.h"


static chsm_result_ten state_3(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten state_1_inner_state(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten state_2(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten state_1(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten menu_structure(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);

static chsm_result_ten menu_structure(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, menu_structure_top, exit_func, guards_only_b);
}

static chsm_result_ten state_1(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {

        default:
            guards_only_b = false;
    }


    return chsm_handle_in_parent(self, ctx_pst, menu_structure, NULL, guards_only_b);
}

static chsm_result_ten state_2(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case SIG_TRANS_1:
            chsm_exit_children(self, e_pst, ctx_pst);
            state1_func(self, e_pst);
            state_1_inner_state_func(self, e_pst);
            return chsm_transition(self, state_1_inner_state);

        default:
            guards_only_b = false;
    }


    return chsm_handle_in_parent(self, ctx_pst, menu_structure, NULL, guards_only_b);
}

static chsm_result_ten state_1_inner_state(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case SIG_TRANS_2:
            chsm_exit_children(self, e_pst, ctx_pst);
            exit_inner_func(self, e_pst);
            state2_func(self, e_pst);
            return chsm_transition(self, state_2);

        case SIG_TRANS_3:
            chsm_exit_children(self, e_pst, ctx_pst);
            exit_inner_func(self, e_pst);
            state_3_func(self, e_pst);
            return chsm_transition(self, state_3);

        case SIG_TRANS_1:
            chsm_exit_children(self, e_pst, ctx_pst);
            exit_inner_func(self, e_pst);
            state_1_inner_state_func(self, e_pst);
            return chsm_transition(self, state_1_inner_state);

        default:
            guards_only_b = false;
    }


    return chsm_handle_in_parent(self, ctx_pst, state_1, exit_inner_func, guards_only_b);
}

static chsm_result_ten state_3(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case SIG_TRANS_1:
            chsm_exit_children(self, e_pst, ctx_pst);
            state1_func(self, e_pst);
            state_1_inner_state_func(self, e_pst);
            return chsm_transition(self, state_1_inner_state);

        default:
            guards_only_b = false;
    }


    return chsm_handle_in_parent(self, ctx_pst, menu_structure, NULL, guards_only_b);
}

chsm_result_ten menu_structure_top(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            chsm_exit_children(self, e_pst, ctx_pst);
            init_func(self, e_pst);
            state1_func(self, e_pst);
            state_1_inner_state_func(self, e_pst);
            return chsm_transition(self, state_1_inner_state);

        default:
            guards_only_b = false;
    }

    return chsm_ignored(self);
}
