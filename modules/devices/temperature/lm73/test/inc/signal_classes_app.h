#ifndef INC_SIGNAL_CLASSES_APP_H
#define INC_SIGNAL_CLASSES_APP_H

#include "crf.h"
#include "signal_classes_if.h"

typedef enum sig_class_app_en {
    SIG_CLASS_I2C_MASTER = SIG_APP_NAMESPACE_START,
    SIG_CLASS_LM73,
} sig_class_app_ten;

#endif // INC_SIGNAL_CLASSES_APP_H