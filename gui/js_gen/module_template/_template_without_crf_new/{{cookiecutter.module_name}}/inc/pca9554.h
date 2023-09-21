/*
 * lm73.h
 *
 *  Created on: 2015 máj. 20
 *      Author: jszeman
 */

#ifndef INC_PCA9554_H_
#define INC_PCA9554_H_

#include <stdint.h>
#include "i2cMaster.h"

#define PCA_A0_HIGH						(1<<0)
#define PCA_A0_LOW						(0)

#define PCA_A1_HIGH						(1<<1)
#define PCA_A1_LOW						(0)

#define PCA_A2_HIGH						(1<<2)
#define PCA_A2_LOW						(0)

enum PCA9554_STATUS {
	PCA9554_READY,									/**< The driver is ready for operation */
	PCA9554_BUSY,									/**< The driver is in the process of reading or writing data. The in field is not valid. */
	PCA9554_NOT_FOUND,								/**< The configured device did not acknowledged its address */
	PCA9554_STARTING								/**< The driver is in the process of initializing the device */
};

enum PCA9554_PART {
	PCA9554 = 0x20,
	PCA9554A = 0x38
};

typedef enum pca9554_sig_ten {
    PCA9554_SIG_NONE,
    PCA9554_SIG_I2C_SUCCESS,
    PCA9554_SIG_I2C_FAIL,
    PCA9554_SIG_INIT,
    PCA9554_SIG_START_READ
} pca9554_sig_ten;

struct pca9554_config
{
	i2cMaster_h							i2cH;		/**< Pointer to a previously configured I2C master driver */
	enum PCA9554_PART					type;		/**< Exact device type. Necessary for address calculation */
	uint16_t							a;			/** Address pin (A0, A1, A2) configuration */
	uint16_t							out;		/** Initial output value */
	uint16_t							polarity;	/** Input pin polarity */
	uint16_t							direction;	/** Pin direction */
};

typedef struct pca9554_t				*pca9554_h;
typedef struct pca9554_t                pca9554_tst;

struct pca9554_t
{
	/* PUBLIC */
	struct pca9554_config				config;
	enum PCA9554_STATUS					status;

	uint16_t							in;
	uint16_t							inputChanged;

	void								(*setOutput)(pca9554_h self, uint16_t o);
	void								(*setDirection)(pca9554_h self, uint16_t d);
	void								(*setPolarity)(pca9554_h self, uint16_t p);
	void								(*readInput)(pca9554_h self);
	void								(*callback_1ms)(pca9554_h self);

	/* PRIVATE */
	struct i2cTransaction_t				transaction;
	uint8_t								wbuff[4];
	uint8_t								rbuff[2];

	pca9554_sig_ten                     signal_en;
	pca9554_sig_ten                      op_en;

	uint32_t                            timer_u32;
	uint32_t                            fault_cnt_u32;

	void                                (*sm_callback)(pca9554_tst *self);
};



void pca9554_init(pca9554_h self);
void pca9554_top(pca9554_tst *self);

void pca9554_set_status(pca9554_tst *self, enum PCA9554_STATUS status_en);
uint16_t pca9554_timeout(pca9554_tst *self, uint32_t timeout_u32);

static inline void pca9554_set_state(pca9554_tst *self, void (*state_pft)(pca9554_tst *self))
{
    self->sm_callback = state_pft;
}

#endif /* INC_PCA9554_H_ */
