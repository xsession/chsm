#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "cevent.h"
#include "cpool.h"
#include "cqueue.h"
#include "chsm.h"
#include "crf.h"
#include <assert.h>

#define CRF_POOL_CNT_MAX 4

void* crf_new_event(crf_tst *self, uint32_t size, signal_t sig)
{
    if (NULL == self->pool_ast) return NULL;

    for(uint32_t i=0; i<self->pool_cnt_u16; i++)
    {
        cpool_tst *pool = self->pool_ast+i;

        if (NULL == pool) return NULL;

        if (pool->esize >= size)
        {
            cevent_tst* e = CPOOL_NEW(pool);
            if (NULL != e)
            {
                e->gc_info = (gc_info_tst){.pool_id = i+1, .ref_cnt = 0};
                e->sig = sig;
                return e;
            }
        }
    }

    return NULL;
}

void crf_gc(crf_tst *self, const cevent_tst* e_pst)
{
    if (NULL == self->pool_ast) return;

    if (0 == e_pst->gc_info.pool_id) return; // Constant (not dinamycally allocated) event.

    CPOOL_GC(self->pool_ast+(e_pst->gc_info.pool_id - 1), e_pst);

    return;
}

/*
 * TODO: implement publish/subscribe method as a fallback, when the application
 * does'n provide a send function for a state macine
 */
void crf_publish(crf_tst *self, const cevent_tst* e)
{
    (void)self;
    (void)e;
}

void crf_post(crf_tst *self, cevent_tst* e, cqueue_tst *q)
{
    (void)self;
    CQUEUE_PUT(q, e);
}

bool crf_step(crf_tst *self)
{
    cevent_tst *e_pst = NULL;
    chsm_tst **hsm_ppst;
    chsm_tst *hsm_pst;
    bool event_found_b = false;

    for (hsm_ppst = self->chsm_ap; *hsm_ppst; hsm_ppst++)
    {
        for(hsm_pst = *hsm_ppst; hsm_pst; hsm_pst = hsm_pst->next)
        {     
            e_pst = (cevent_tst *)CQUEUE_GET(&hsm_pst->event_q_st);
            if (e_pst)
            {
                event_found_b = true;

                hsm_pst->state_handler_pft(hsm_pst, e_pst);

                if (0 == e_pst->gc_info.ref_cnt)
                {
                    crf_gc(self, e_pst);
                }
            }
        }
    }

    return event_found_b;
}

bool crf_init(crf_tst *self , chsm_tst **chsm_ap, cpool_tst *pool_ast, uint16_t pool_cnt)
{
    if (NULL == self) return false;

#ifndef CHSM_CFG_LITE
    self->new_event = crf_new_event;
    self->publish = crf_publish;
    self->post = crf_post;
    self->step = crf_step;
    self->gc = crf_gc;
#endif

    if (NULL == chsm_ap) return false;

    self->chsm_ap = chsm_ap;
    self->pool_ast = pool_ast;
    self->pool_cnt_u16 = pool_cnt;

    return true;
}
