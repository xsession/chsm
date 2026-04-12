/*
 * event_queue.h
 *
 *  Created on: 2019. jan. 26.
 *      Author: jszeman
 */

#ifndef INC_CQUEUE_H
#define INC_CQUEUE_H

#include <stdint.h>
#include "cevent.h"
#include "atomic_ops.h"
#include "chsm_cfg.h"

/*
 * Thread safe multi-producer single consumer event queue
 * 
 * The application should only ever need to call the put method directly
 * to place events into the queue. Calling this function from interrupts
 * and other threads is safe.
 * All the other methods sould only be called from the main thread.
 */

typedef struct cqueue_tst cqueue_tst;

struct cqueue_tst
{
#ifndef CHSM_CFG_LITE
    int32_t             (*put)(cqueue_tst *self, const cevent_tst *e_cpst);
    int32_t             (*put_left)(cqueue_tst *self, const cevent_tst *e_cpst);
    const cevent_tst*   (*get)(cqueue_tst *self);
    const cevent_tst*   (*get_right)(cqueue_tst *self);
#endif

    const cevent_tst    **events;
    uint16_t            max;
    atomic_uint16_t     head;
    atomic_uint16_t     tail;
    uint16_t            mask;
#ifndef CHSM_CFG_NO_FAULT_CNT
    uint32_t            fault_cnt;
#endif
};

int32_t                 cqueue_init(cqueue_tst *self, const cevent_tst **events, uint16_t max_event_count);

/* Direct-call function declarations (always available) */
int32_t                 cqueue_put(cqueue_tst *self, const cevent_tst *e);
int32_t                 cqueue_put_left(cqueue_tst *self, const cevent_tst *e_pst);
const cevent_tst*       cqueue_get(cqueue_tst *self);
const cevent_tst*       cqueue_get_right(cqueue_tst *self);

/*
 * Dispatch macros: use direct calls when CHSM_CFG_LITE, else pointer dispatch.
 */
#ifdef CHSM_CFG_LITE
    #define CQUEUE_PUT(q, e)        cqueue_put((q), (e))
    #define CQUEUE_PUT_LEFT(q, e)   cqueue_put_left((q), (e))
    #define CQUEUE_GET(q)           cqueue_get((q))
    #define CQUEUE_GET_RIGHT(q)     cqueue_get_right((q))
#else
    #define CQUEUE_PUT(q, e)        (q)->put((q), (e))
    #define CQUEUE_PUT_LEFT(q, e)   (q)->put_left((q), (e))
    #define CQUEUE_GET(q)           (q)->get((q))
    #define CQUEUE_GET_RIGHT(q)     (q)->get_right((q))
#endif

#endif /* INC_CQUEUE_H */
