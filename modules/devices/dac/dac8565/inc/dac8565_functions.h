#ifndef DAC8565_FUNCTIONS_H
#define DAC8565_FUNCTIONS_H

/*Generated with CHSM v0.0.0 at 2023.03.19 07.19.32*/

#include "dac8565.h"
#include "chsm.h"
#include "cevent.h"
#include <stdbool.h>


void dac8565_reset_reg_ptr(chsm_tst *self, const cevent_tst *e_pst);

void dac8565_send_next(chsm_tst *self, const cevent_tst *e_pst);


typedef enum dac8565_state_id_ten
{
    S_DAC8565_IDLE = 1,
    S_DAC8565_SEND_NEXT = 3,
} dac8565_state_id_ten;


/*
Signals:
    SIG_SPI_DRIVER_TRANSFER_SUCCESS 
*/

/*
Other function notes:
*/
#endif
