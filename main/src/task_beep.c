#define TAG "BEEP"

#include "device/buzzer.h"
#include "esp_log.h"
#include "esp_system.h"
#include "tasks.h"

void
task_beep(void *arg)
{
	while (1) {
		ESP_LOGI(TAG, "beep");
		buzzer_set(1);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		buzzer_set(0);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}
