#define TAG "MAIN"

#include "device/backlight.h"
#include "device/buzzer.h"
#include "esp_log.h"
#include "esp_system.h"

void
app_main()
{
	buzzer_init();

	bl_init();
	bl_set(16);

	ESP_LOGI(TAG, "SYSTEM READY");
}
