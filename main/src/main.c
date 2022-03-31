#define TAG "MAIN"

#include "device/backlight.h"
#include "device/buzzer.h"
#include "device/rgb.h"
#include "esp_log.h"
#include "esp_system.h"

void
app_main()
{
	buzzer_init();

	bl_init();
	bl_set(16);

	rgb_init();
	rgb_set(0x0000FF);

	ESP_LOGI(TAG, "SYSTEM READY");
}
