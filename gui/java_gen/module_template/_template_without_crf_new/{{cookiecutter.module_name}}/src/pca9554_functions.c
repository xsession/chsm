/*
 * lm73.c
 *
 *  Created on: 2015 máj. 20
 *      Author: jszeman
 */

#include <stdint.h>
#include "i2cMaster.h"
#include "math.h"
#include "pca9554.h"

#define PCA_REG_INPUT					0
#define PCA_REG_OUTPUT					1
#define PCA_REG_POLARITY				2
#define PCA_REG_DIRECTION				3

static void startRegWrite(pca9554_h self)
{
	self->transaction.writeLength = 2;			// write pointer byte
	self->transaction.readLength = 0;
	self->config.i2cH->start(self->config.i2cH, &self->transaction);

	//self->status = PCA9554_BUSY;
}

static void setOutput(pca9554_h self, uint16_t o)
{
	self->wbuff[0] = PCA_REG_OUTPUT;
	self->wbuff[1] = o;

	startRegWrite(self);
}

static void setDirection(pca9554_h self, uint16_t d)
{
	self->wbuff[0] = PCA_REG_DIRECTION;
	self->wbuff[1] = d;

	startRegWrite(self);
}

static void setPolarity(pca9554_h self, uint16_t p)
{
	self->wbuff[0] = PCA_REG_POLARITY;
	self->wbuff[1] = p;

	startRegWrite(self);
}

static void readInput(pca9554_h self)
{
	self->wbuff[0] = PCA_REG_INPUT;
	self->transaction.writeLength = 1;			// write pointer byte
	self->transaction.readLength = 1;
	self->config.i2cH->start(self->config.i2cH, &self->transaction);

	self->status = PCA9554_BUSY;
}

static void start_read(pca9554_h self)
{
    self->op_en = PCA9554_SIG_START_READ;
}

static void callback(pca9554_h self)
{
    self->signal_en = PCA9554_SIG_NONE;

    switch (self->transaction.result)
    {
        case I2C_SUCCESS:
            self->signal_en = PCA9554_SIG_I2C_SUCCESS;
            self->transaction.result = I2C_NONE;
            break;

        case I2C_ADDRESS_NACK:
        case I2C_DATA_NACK:
            self->signal_en = PCA9554_SIG_I2C_FAIL;
            self->transaction.result = I2C_NONE;
            self->fault_cnt_u32++;
            break;

        default:
            self->signal_en = self->op_en;
            self->op_en = PCA9554_SIG_NONE;
    }

    self->sm_callback(self);

    self->timer_u32++;
}

void pca9554_set_status(pca9554_tst *self, enum PCA9554_STATUS status_en)
{
    self->status = status_en;
}

void pca9554_process_input(pca9554_tst *self)
{
    self->in = self->rbuff[0];
    self->inputChanged = 1;
}

void pca9554_read_input(pca9554_tst *self)
{
    readInput(self);
}

void pca9554_set_direction(pca9554_tst *self)
{
    setDirection(self, self->config.direction);
}

void pca9554_set_output(pca9554_tst *self)
{
    setOutput(self, self->config.out);
}

void pca9554_set_polarity(pca9554_tst *self)
{
    setPolarity(self, self->config.polarity);
}

void pca9554_reset_timer(pca9554_tst *self)
{
    self->timer_u32 = 0;
}

uint16_t pca9554_timeout(pca9554_tst *self, uint32_t timeout_u32)
{
    return self->timer_u32 >= timeout_u32;
}

void pca9554_init(pca9554_h self)
{
	self->setOutput = setOutput;
	self->setDirection = setDirection;
	self->setPolarity = setPolarity;
	self->readInput = start_read;
	self->callback_1ms = callback;

	self->transaction.address = self->config.type | (self->config.a & 0x07);
	self->transaction.result = I2C_NONE;
	self->transaction.readData = self->rbuff;
	self->transaction.writeData = self->wbuff;
	self->inputChanged = 0;
	self->fault_cnt_u32 = 0;

	self->sm_callback = pca9554_top;
	self->op_en = PCA9554_SIG_INIT;

}
