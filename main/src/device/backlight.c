#define TAG "BL"

#include "backlight.h"

#include "driver/mcpwm.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pinout.h"

static int current_level;

void
bl_init(void)
{
	gpio_config_t output = {
		.pin_bit_mask = (1UL << PIN_BLK),
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = GPIO_PULLUP_ENABLE,
	};

	ESP_ERROR_CHECK(gpio_config(&output));
}

void
bl_set(uint8_t level)
{
	if (level > 16) level = 16;
	if (level == 0) {
		gpio_set_level(PIN_BLK, 0);
		vTaskDelay(3 / portTICK_PERIOD_MS);
		ESP_LOGD(TAG, "off");
	} else {
		if (current_level == 0) {
			gpio_set_level(PIN_BLK, 1);
			vTaskDelay(1 / portTICK_PERIOD_MS);
		}
		int pul_from = 16 - current_level;
		int pul_to = 16 - level;
		int pul_num = (16 + pul_to - pul_from) % 16;
		for (int i = 0; i < pul_num; i++) {
			gpio_set_level(PIN_BLK, 0);
			gpio_set_level(PIN_BLK, 1);
		}
		ESP_LOGD(TAG, "level: %d, pulse from %d to %d, num: %d", level, pul_from, pul_to, pul_num);
	}
	current_level = level;
}
