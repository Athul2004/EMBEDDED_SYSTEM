#ifndef SMS_CONTROLLER_H
#define SMS_CONTROLLER_H

#include <stdbool.h>

extern volatile bool sms_trigger_pending;

void sms_controller_start(void);

#endif // SMS_CONTROLLER_H
