#ifndef PCA9554_FUNCTIONS_H
#define PCA9554_FUNCTIONS_H

/*Generated with CHSM v0.0.0 at 2023.07.27 14.07.05*/

#include "pca9554.h"
#include <stdbool.h>


void pca9554_process_input(pca9554_tst *self);

void pca9554_read_input(pca9554_tst *self);

void pca9554_reset_timer(pca9554_tst *self);

/*Start a I2C transaction to write the DIRECTION register in the slave device.*/
void pca9554_set_direction(pca9554_tst *self);

void pca9554_set_output(pca9554_tst *self);

void pca9554_set_polarity(pca9554_tst *self);


typedef enum pca9554_state_id_ten
{
    S_PCA9554_WAIT_DIR = 1,
    S_PCA9554_WAIT_POL = 2,
    S_PCA9554_WAIT_OUTPUT = 3,
    S_PCA9554_WAIT_READ = 4,
    S_PCA9554_BACK_OFF = 5,
} pca9554_state_id_ten;


/*
Signals:
    I2C_FAIL    
    I2C_SUCCESS 
    START_READ  
*/

/*
Other function notes:

pca9554_set_status:

pca9554_timeout:
*/
#endif
