#define TAG "RGB"

#include "rgb.h"

#include "driver/rmt.h"
#include "esp_log.h"
#include "esp_system.h"
#include "led_strip.h"
#include "pinout.h"

#define RMT_TX_CHANNEL RMT_CHANNEL_0

static led_strip_t *strip = NULL;
static uint32_t last_color = 0;

void
rgb_init(void)
{
	rmt_config_t config = RMT_DEFAULT_CONFIG_TX(PIN_RMT_RGB, RMT_TX_CHANNEL);
	config.clk_div = 2;

	ESP_ERROR_CHECK(rmt_config(&config));
	ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

	led_strip_config_t strip_config =
		LED_STRIP_DEFAULT_CONFIG(PIN_RMT_RGB, (led_strip_dev_t)config.channel);

	strip = led_strip_new_rmt_ws2812(&strip_config);
	if (strip == NULL) {
		ESP_LOGE(TAG, "Install WS2812 driver failed");
	}

	ESP_ERROR_CHECK(strip->clear(strip, 100));
}

void
rgb_set(uint32_t color)
{
	if (strip == NULL) {
		ESP_LOGE(TAG, "RGB not ready");
		return;
	}

	if (last_color != color) {
		last_color = color;
		ESP_ERROR_CHECK(strip->set_pixel(strip, 0, GET_R(color), GET_G(color), GET_B(color)));
		ESP_ERROR_CHECK(strip->refresh(strip, 100));
	}
}
