#ifndef MENU_STRUCTURE_H
#define MENU_STRUCTURE_H

#include "stdint.h"
#include "crf.h"
#include "sys_if.h"
#include "signal_classes.h"

/*
 * SIGNALS
 */

typedef enum menu_structure_signals_ten
{
    SIG_MENU_STRUCTURE_NONE = SIGNAL_FROM_CLASS(SIG_CLASS_MENU_STRUCTURE),
    SIG_TRANS_1,
    SIG_TRANS_2,
    SIG_TRANS_3
} menu_structure_signals_ten;

#define SIG_TRANS_1_TYPE                 cevent_tst
#define SIG_TRANS_2_TYPE                 cevent_tst
#define SIG_TRANS_3_TYPE                 cevent_tst


/*
 * EVENT DEFINITIONS
 */
typedef struct menu_structure_status_tst
{
    cevent_tst          super;              // Signal and GC stuff
} menu_structure_status_tst;

/*
 * ACTIVE OBJECT
 */



struct menu_structure_st
{
    /* PUBLIC */
    chsm_tst            super;


    /* PRIVATE */

}menu_structure_st;

typedef struct menu_structure_st menu_structure_tst;

menu_structure_tst menu_tst;

chsm_result_ten menu_structure_top(chsm_tst *self, const cevent_tst  *e_pst, chsm_call_ctx_tst *ctx_pst);

#endif
