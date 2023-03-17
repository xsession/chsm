#ifndef DEFAULT_SIGNALS_H
#define DEFAULT_SIGNALS_H

#include "crf.h"
#include "signal_classes_if.h"

typedef enum sig_class_app_en {
    SIG_CLASS_I2C_MASTER = SIG_APP_NAMESPACE_START,
    SIG_CLASS_SPI_MASTER,
    // SIG_CLASS_CANOPEN,
    SIG_CLASS_LM73,
    SIG_EXT_APP_NAMESPACE_START,
} sig_class_app_ten;


#endif // DEFAULT_SIGNALS_H