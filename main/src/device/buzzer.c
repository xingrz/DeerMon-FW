#define TAG "BUZZER"

#include "buzzer.h"

#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_system.h"
#include "pinout.h"

#define BUZZ_LEDC_TIMER LEDC_TIMER_0
#define BUZZ_LEDC_MODE LEDC_HIGH_SPEED_MODE
#define BUZZ_LEDC_CHANNEL LEDC_CHANNEL_0
#define BUZZ_LEDC_RES LEDC_TIMER_3_BIT
#define BUZZ_LEDC_FREQ 10 * 1000 * 1000

void
buzzer_init(void)
{
	ledc_timer_config_t timer = {
		.duty_resolution = BUZZ_LEDC_RES,
		.freq_hz = BUZZ_LEDC_FREQ,
		.speed_mode = BUZZ_LEDC_MODE,
		.timer_num = BUZZ_LEDC_TIMER,
		.clk_cfg = LEDC_AUTO_CLK,
	};

	ledc_timer_config(&timer);

	ledc_channel_config_t channel = {
		.channel = BUZZ_LEDC_CHANNEL,
		.duty = 0,
		.gpio_num = PIN_BZ_EN,
		.speed_mode = BUZZ_LEDC_MODE,
		.hpoint = 0,
		.timer_sel = BUZZ_LEDC_TIMER,
	};

	ledc_channel_config(&channel);
}

void
buzzer_set(uint8_t level)
{
	level = level & 0x07;  // 3 bits, 0 ~ 7
	ESP_LOGD(TAG, "buzz: %d", level);
	ledc_set_duty(BUZZ_LEDC_MODE, BUZZ_LEDC_CHANNEL, level);
	ledc_update_duty(BUZZ_LEDC_MODE, BUZZ_LEDC_CHANNEL);
}
