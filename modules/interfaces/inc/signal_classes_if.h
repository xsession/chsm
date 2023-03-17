#ifndef INC_SIGNAL_CLASSES_IF_H
#define INC_SIGNAL_CLASSES_IF_H

#include "crf.h"

typedef enum sig_class_if_en {
    SIG_CLASS_SYS = CRF_SIGNAL_CLASS_START,
    SIG_CLASS_I2C_DRIVER,
    SIG_CLASS_SPI_DRIVER,
    SIG_CLASS_MEM,
    SIG_CLASS_CAN,
    SIG_APP_NAMESPACE_START,
} sig_class_if_ten;

#endif // INC_SIGNAL_CLASSES_IF_H