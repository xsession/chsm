/*
 * chsm_cfg.h
 *
 * CHSM Runtime Configuration
 *
 * Include this header BEFORE any CRF header, or define the macros
 * via compiler flags (-DCHSM_CFG_LITE, etc.)
 *
 * CHSM_CFG_LITE
 *      Strip function-pointer dispatch from cqueue, cpool, and crf structs.
 *      All calls become direct (allows inlining, saves RAM per instance).
 *      Safe when there is only one implementation per function (default).
 *
 * CHSM_CFG_NO_FAULT_CNT
 *      Remove the fault_cnt field from cqueue_tst (saves 4 bytes / queue).
 *
 * CHSM_CFG_NO_DEBUG
 *      Code-generator will emit empty debug_log_func and omit the per-module
 *      debug_state_ac[] buffer (saves 20 bytes RAM + flash per module).
 */

#ifndef CHSM_CFG_H_
#define CHSM_CFG_H_

/*
 * Uncomment the options you need, or pass them as compiler defines.
 * Example:  -DCHSM_CFG_LITE -DCHSM_CFG_NO_FAULT_CNT -DCHSM_CFG_NO_DEBUG
 */

/* #define CHSM_CFG_LITE */
/* #define CHSM_CFG_NO_FAULT_CNT */
/* #define CHSM_CFG_NO_DEBUG */

#endif /* CHSM_CFG_H_ */
