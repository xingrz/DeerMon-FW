#ifndef __PROJECT_TASKS__
#define __PROJECT_TASKS__

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#define TASK_LV_STACK_SIZE (8 * 1024)
#define TASK_UI_STACK_SIZE (8 * 1024)
#define TASK_BEEP_STACK_SIZE (2 * 1024)

#define TASK_LV_PRIO (tskIDLE_PRIORITY + 1)
#define TASK_UI_PRIO (tskIDLE_PRIORITY + 1)
#define TASK_BEEP_PRIO (tskIDLE_PRIORITY + 1)

void task_lv(void *arg);
void task_ui(void *arg);
void task_beep(void *arg);

#endif  // __PROJECT_TASKS__
