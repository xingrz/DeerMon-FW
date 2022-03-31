#define TAG "MAIN"

#include "device/buzzer.h"
#include "esp_log.h"
#include "esp_system.h"

void
app_main()
{
	buzzer_init();

	ESP_LOGI(TAG, "SYSTEM READY");
}
