/*Generated with CHSM v0.0.1*/
#include "pca9554.h"
#include "pca9554_functions.h"


static void s_pca9554_back_off(pca9554_tst *self);
static void s_pca9554_wait_read(pca9554_tst *self);
static void s_pca9554_wait_output(pca9554_tst *self);
static void s_pca9554_wait_pol(pca9554_tst *self);
static void s_pca9554_wait_dir(pca9554_tst *self);

static void s_pca9554_wait_dir(pca9554_tst *self)
{
    switch(self->signal_en)
    {
        case PCA9554_SIG_I2C_SUCCESS:
            pca9554_set_polarity(self);
            return pca9554_set_state(self, s_pca9554_wait_pol);

        case PCA9554_SIG_I2C_FAIL:
            pca9554_set_status(self, PCA9554_NOT_FOUND);
            pca9554_reset_timer(self);
            return pca9554_set_state(self, s_pca9554_back_off);
    }

    return ;
}

static void s_pca9554_wait_pol(pca9554_tst *self)
{
    switch(self->signal_en)
    {
        case PCA9554_SIG_I2C_SUCCESS:
            pca9554_set_output(self);
            return pca9554_set_state(self, s_pca9554_wait_output);

        case PCA9554_SIG_I2C_FAIL:
            pca9554_set_status(self, PCA9554_NOT_FOUND);
            pca9554_reset_timer(self);
            return pca9554_set_state(self, s_pca9554_back_off);
    }

    return ;
}

static void s_pca9554_wait_output(pca9554_tst *self)
{
    switch(self->signal_en)
    {
        case PCA9554_SIG_I2C_SUCCESS:
            pca9554_read_input(self);
            return pca9554_set_state(self, s_pca9554_wait_read);

        case PCA9554_SIG_I2C_FAIL:
            pca9554_set_status(self, PCA9554_NOT_FOUND);
            pca9554_reset_timer(self);
            return pca9554_set_state(self, s_pca9554_back_off);
    }

    return ;
}

static void s_pca9554_wait_read(pca9554_tst *self)
{
    switch(self->signal_en)
    {
        case PCA9554_SIG_I2C_SUCCESS:
            pca9554_set_status(self, PCA9554_READY);
            pca9554_process_input(self);
            break;

        case PCA9554_SIG_START_READ:
            pca9554_read_input(self);
            return pca9554_set_state(self, s_pca9554_wait_read);

        case PCA9554_SIG_I2C_FAIL:
            pca9554_set_status(self, PCA9554_NOT_FOUND);
            pca9554_reset_timer(self);
            return pca9554_set_state(self, s_pca9554_back_off);
    }

    return ;
}

static void s_pca9554_back_off(pca9554_tst *self)
{
    switch(self->signal_en)
    {
        case PCA9554_SIG_I2C_FAIL:
            pca9554_set_status(self, PCA9554_NOT_FOUND);
            pca9554_reset_timer(self);
            return pca9554_set_state(self, s_pca9554_back_off);
    }

    if(pca9554_timeout(self, 1000))
    {
        pca9554_set_direction(self);
        return pca9554_set_state(self, s_pca9554_wait_dir);
    }

    return ;
}

void pca9554_top(pca9554_tst *self)
{
    switch(self->signal_en)
    {
        case PCA9554_SIG_INIT:
            pca9554_set_status(self, PCA9554_STARTING);
            pca9554_set_direction(self);
            return pca9554_set_state(self, s_pca9554_wait_dir);
    }

    return ;
}
