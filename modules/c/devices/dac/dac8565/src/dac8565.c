/*Generated with CHSM v0.0.0 at 2023.03.19 07.19.32*/
#include "cevent.h"
#include "chsm.h"
#include "dac8565.h"
#include "dac8565_functions.h"


static chsm_result_ten s_dac8565_send_next(chsm_tst *self, const cevent_tst  *e_pst);
static chsm_result_ten s_dac8565_idle(chsm_tst *self, const cevent_tst  *e_pst);

static chsm_result_ten s_dac8565_idle(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
    }

    dac8565_reset_reg_ptr(self, e_pst);
    dac8565_send_next(self, e_pst);
    return chsm_transition(self, s_dac8565_send_next);

    return chsm_ignored(self);
}

static chsm_result_ten s_dac8565_send_next(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case SIG_SPI_DRIVER_TRANSFER_SUCCESS:
            dac8565_send_next(self, e_pst);
            return chsm_transition(self, s_dac8565_send_next);
    }

    return chsm_ignored(self);
}

chsm_result_ten dac8565_top(chsm_tst *self, const cevent_tst  *e_pst)
{
    switch(e_pst->sig)
    {
        case C_SIG_INIT:
            return chsm_transition(self, s_dac8565_idle);
    }

    return chsm_ignored(self);
}
