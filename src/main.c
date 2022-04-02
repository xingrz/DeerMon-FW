#include <device.h>
#include <drivers/led_strip.h>
#include <logging/log.h>
#include <zephyr.h>

LOG_MODULE_REGISTER(main);

static const struct device *status_rgb = DEVICE_DT_GET(DT_ALIAS(status_rgb));

void
main(void)
{
	if (!device_is_ready(status_rgb)) {
		LOG_ERR("RGB LED device %s is not ready", status_rgb->name);
		return;
	}

	struct led_rgb rgb = {.r = 0x00, .g = 0x00, .b = 0xFF};
	led_strip_update_rgb(status_rgb, &rgb, 1);
}
