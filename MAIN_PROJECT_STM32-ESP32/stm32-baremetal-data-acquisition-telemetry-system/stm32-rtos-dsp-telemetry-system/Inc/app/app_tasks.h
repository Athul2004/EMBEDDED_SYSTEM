#ifndef APP_TASKS_H
#define APP_TASKS_H

#include <stdint.h>
#include "dsp/dsp_engine.h"

extern DSP_Engine_t dsp;
extern volatile uint8_t trigger_sms_from_dashboard;

void AppTasks_Init(void);

#endif // APP_TASKS_H
