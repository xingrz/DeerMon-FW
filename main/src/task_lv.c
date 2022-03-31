#define TAG "LV"

#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "lvgl.h"
#include "tasks.h"

#define LVGL_TICK_MS 1

static void
lv_tick_timercb(void *timer)
{
	lv_tick_inc(LVGL_TICK_MS);
}

void
task_lv(void *arg)
{
	esp_timer_create_args_t timer_conf = {
		.callback = lv_tick_timercb,
		.name = "lv_tick_timer",
	};
	esp_timer_handle_t timer = NULL;
	esp_timer_create(&timer_conf, &timer);
	esp_timer_start_periodic(timer, LVGL_TICK_MS * 1000U);

	SemaphoreHandle_t xGuiSemaphore = xSemaphoreCreateMutex();
	while (1) {
		vTaskDelay(pdMS_TO_TICKS(30));
		if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
			lv_task_handler();
			xSemaphoreGive(xGuiSemaphore);
		}
	}

	vTaskDelete(NULL);
}
