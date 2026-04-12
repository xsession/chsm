/*
 * event_pool.h
 *
 *  Created on: 2019. jan. 25.
 *      Author: jszeman
 */

#ifndef INC_CPOOL_H_
#define INC_CPOOL_H_

#include <stdint.h>
#include <stdbool.h>
#include "cevent.h"
#include "atomic_ops.h"
#include "chsm_cfg.h"
#include <limits.h>

#if (CHAR_BIT == 16)
    #ifndef COMMON_TYPES_H_
        typedef unsigned char       uint8_t;
        typedef char                int8_t;
    #endif
#endif
#if (CHAR_BIT == 8)
typedef unsigned char               uint8;
#endif

typedef struct cpool_tst cpool_tst;

struct cpool_tst
{
    uint8_t             *pool;  // Pointer to a user allocated buffer
    uint16_t            esize;  // Unit size of events in the pool
    uint16_t            ecnt;   // Max number of events in the pool
    atomic_uint16_t     head;   // Offset of the first free block

#ifndef CHSM_CFG_LITE
    void*               (*new)(cpool_tst *self);
    bool                (*gc)(cpool_tst *self, const cevent_tst *e);
#endif
};

void        cpool_init(cpool_tst *self, uint8_t *buff, uint16_t event_size, uint16_t event_count);

/* Direct-call function declarations (always available) */
void*       cpool_new(cpool_tst *self);
bool        cpool_gc(cpool_tst *self, const cevent_tst *e_pst);

#ifdef CHSM_CFG_LITE
    #define CPOOL_NEW(p)        cpool_new((p))
    #define CPOOL_GC(p, e)      cpool_gc((p), (e))
#else
    #define CPOOL_NEW(p)        (p)->new((p))
    #define CPOOL_GC(p, e)      (p)->gc((p), (e))
#endif

#endif /* INC_CPOOL_H_ */
