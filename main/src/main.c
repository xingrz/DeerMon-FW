#define TAG "MAIN"

#include "device/backlight.h"
#include "device/buzzer.h"
#include "device/rgb.h"
#include "device/st7735s.h"
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

	st7735s_init();
	st7735s_fill(0, ST7735S_WIDTH - 1, 0, ST7735S_HEIGHT - 1, RGB565(0xFFFFFF));

	ESP_LOGI(TAG, "SYSTEM READY");
}
