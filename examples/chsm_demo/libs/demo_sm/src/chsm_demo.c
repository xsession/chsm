/*Generated with CHSM v0.0.0 at 2022.10.13 13.49.38*/
#include "cevent.h"
#include "chsm.h"
#include "chsm_demo.h"
#include "chsm_demo_functions.h"


static chsm_result_ten s4(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s3(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s211(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s21(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s2(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s11(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s1(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);
static chsm_result_ten s(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);

static chsm_result_ten s(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case E:
            chsm_exit_children(self, e_pst, ctx_pst);
            s1_entry(self, e_pst);
            s11_entry(self, e_pst);
            s11_init(self, e_pst);
            return chsm_transition(self, s11);

        case J:
            chsm_exit_children(self, e_pst, ctx_pst);
            chsm_recall(self, e_pst);
            return chsm_transition(self, s3);

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, chsm_demo, s_exit, guards_only_b);
}

static chsm_result_ten s1(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case ID:
            s1_func(self, e_pst);
            break;

        case C:
            chsm_exit_children(self, e_pst, ctx_pst);
            s1_exit(self, e_pst);
            s2_entry(self, e_pst);
            s2_init(self, e_pst);
            s21_entry(self, e_pst);
            s21_init(self, e_pst);
            s211_entry(self, e_pst);
            s211_init(self, e_pst);
            return chsm_transition(self, s211);

        case F:
            chsm_exit_children(self, e_pst, ctx_pst);
            s1_exit(self, e_pst);
            s2_entry(self, e_pst);
            s21_entry(self, e_pst);
            s211_entry(self, e_pst);
            s211_init(self, e_pst);
            return chsm_transition(self, s211);

        case B:
            chsm_exit_children(self, e_pst, ctx_pst);
            s11_entry(self, e_pst);
            s11_init(self, e_pst);
            return chsm_transition(self, s11);

        case A:
            chsm_exit_children(self, e_pst, ctx_pst);
            s1_exit(self, e_pst);
            s1_entry(self, e_pst);
            s1_init(self, e_pst);
            s11_entry(self, e_pst);
            s11_init(self, e_pst);
            return chsm_transition(self, s11);

        default:
            guards_only_b = false;
    }

    if(s1_guard(self, e_pst))
    {
        s1_func(self, e_pst);
    }

    if(j_guard(self, e_pst))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        s1_exit(self, e_pst);
        s2_entry(self, e_pst);
        s2_init(self, e_pst);
        s21_entry(self, e_pst);
        s21_init(self, e_pst);
        s211_entry(self, e_pst);
        s211_init(self, e_pst);
        return chsm_transition(self, s211);
    }

    return chsm_handle_in_parent(self, ctx_pst, s, s1_exit, guards_only_b);
}

static chsm_result_ten s11(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case ID:
            s11_id(self, e_pst, 8);
            break;

        case G:
            if(s11_g_guard1(self, e_pst, 1))
            {
                s11_g1(self, e_pst, 2);
            }
            if(s11_g_guard2(self, e_pst, 4))
            {
                chsm_exit_children(self, e_pst, ctx_pst);
                s11_g2(self, e_pst, 3);
                s11_exit(self, e_pst);
                s1_exit(self, e_pst);
                s2_entry(self, e_pst);
                s21_entry(self, e_pst);
                s211_entry(self, e_pst);
                s211_init(self, e_pst);
                return chsm_transition(self, s211);
            }
            break;

        case K:
            chsm_defer(self, e_pst);
            break;

        case L:
            chsm_defer(self, e_pst);
            break;

        case D:
            if(cond(self, e_pst))
            {
                chsm_exit_children(self, e_pst, ctx_pst);
                d_func(self, e_pst);
                s11_exit(self, e_pst);
                s1_init(self, e_pst);
                s11_entry(self, e_pst);
                s11_init(self, e_pst);
                return chsm_transition(self, s11);
            }
            break;

        case H:
            chsm_exit_children(self, e_pst, ctx_pst);
            s11_exit(self, e_pst);
            s1_exit(self, e_pst);
            s_init(self, e_pst);
            s1_entry(self, e_pst);
            s1_init(self, e_pst);
            s11_entry(self, e_pst);
            s11_init(self, e_pst);
            return chsm_transition(self, s11);

        default:
            guards_only_b = false;
    }

    if(s11_guard(self, e_pst, 6))
    {
        s11_func(self, e_pst, 7);
    }

    if(k_guard(self, e_pst, 5))
    {
        chsm_exit_children(self, e_pst, ctx_pst);
        s11_exit(self, e_pst);
        s1_exit(self, e_pst);
        s2_entry(self, e_pst);
        s2_init(self, e_pst);
        s21_entry(self, e_pst);
        s21_init(self, e_pst);
        s211_entry(self, e_pst);
        s211_init(self, e_pst);
        return chsm_transition(self, s211);
    }

    return chsm_handle_in_parent(self, ctx_pst, s1, s11_exit, guards_only_b);
}

static chsm_result_ten s2(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case C:
            chsm_exit_children(self, e_pst, ctx_pst);
            s2_exit(self, e_pst);
            s1_entry(self, e_pst);
            s1_init(self, e_pst);
            s11_entry(self, e_pst);
            s11_init(self, e_pst);
            return chsm_transition(self, s11);

        case F:
            chsm_exit_children(self, e_pst, ctx_pst);
            s2_exit(self, e_pst);
            s1_entry(self, e_pst);
            s11_entry(self, e_pst);
            s11_init(self, e_pst);
            return chsm_transition(self, s11);

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, s, s2_exit, guards_only_b);
}

static chsm_result_ten s21(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case G:
            chsm_exit_children(self, e_pst, ctx_pst);
            s21_exit(self, e_pst);
            s2_exit(self, e_pst);
            s1_entry(self, e_pst);
            s1_init(self, e_pst);
            s11_entry(self, e_pst);
            s11_init(self, e_pst);
            return chsm_transition(self, s11);

        case B:
            chsm_exit_children(self, e_pst, ctx_pst);
            s211_entry(self, e_pst);
            s211_init(self, e_pst);
            return chsm_transition(self, s211);

        case A:
            chsm_exit_children(self, e_pst, ctx_pst);
            s21_exit(self, e_pst);
            s21_entry(self, e_pst);
            s21_init(self, e_pst);
            s211_entry(self, e_pst);
            s211_init(self, e_pst);
            return chsm_transition(self, s211);

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, s2, s21_exit, guards_only_b);
}

static void s211_exit_func_wrapper(chsm_tst *self, const cevent_tst *e_pst)
{
    s211_exit(self, e_pst, 6);
}

static chsm_result_ten s211(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case ID:
            s211_id(self, e_pst);
            break;

        case H:
            chsm_exit_children(self, e_pst, ctx_pst);
            s211_exit(self, e_pst, 6);
            s21_exit(self, e_pst);
            s2_exit(self, e_pst);
            s_init(self, e_pst);
            s1_entry(self, e_pst);
            s1_init(self, e_pst);
            s11_entry(self, e_pst);
            s11_init(self, e_pst);
            return chsm_transition(self, s11);

        case D:
            chsm_exit_children(self, e_pst, ctx_pst);
            s211_exit(self, e_pst, 6);
            s21_init(self, e_pst);
            s211_entry(self, e_pst);
            s211_init(self, e_pst);
            return chsm_transition(self, s211);

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, s21, s211_exit_func_wrapper, guards_only_b);
}

static chsm_result_ten s3(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case K:
            s3_k_func(self, e_pst);
            break;

        case L:
            s3_l_func(self, e_pst);
            break;

        case J:
            chsm_exit_children(self, e_pst, ctx_pst);
            s_init(self, e_pst);
            s1_entry(self, e_pst);
            s1_init(self, e_pst);
            s11_entry(self, e_pst);
            s11_init(self, e_pst);
            return chsm_transition(self, s11);

        case G:
            if(s11_g_guard2(self, e_pst, 4))
            {
                chsm_exit_children(self, e_pst, ctx_pst);
                s11_g2(self, e_pst, 3);
                return chsm_transition(self, s4);
            }
            if(s11_g_guard1(self, e_pst, 1))
            {
                chsm_exit_children(self, e_pst, ctx_pst);
                s11_g1(self, e_pst, 2);
                return chsm_transition(self, s4);
            }
            break;

        default:
            guards_only_b = false;
    }


    return chsm_handle_in_parent(self, ctx_pst, s, NULL, guards_only_b);
}

static chsm_result_ten s4(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case ID:
            s4_id(self, e_pst);
            break;

        default:
            guards_only_b = false;
    }

    return chsm_handle_in_parent(self, ctx_pst, s, NULL, guards_only_b);
}

chsm_result_ten chsm_demo(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst)
{
    bool guards_only_b=true;
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            chsm_exit_children(self, e_pst, ctx_pst);
            s_entry(self, e_pst);
            s_init(self, e_pst);
            s1_entry(self, e_pst);
            s1_init(self, e_pst);
            s11_entry(self, e_pst);
            s11_init(self, e_pst);
            return chsm_transition(self, s11);

        default:
            guards_only_b = false;
    }

    return chsm_ignored(self);
}
