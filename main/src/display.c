#define TAG "DISPLAY"
#define DISP_DEBUG 0

#include "display.h"

#include <stdint.h>

#include "device/st7735s.h"
#include "esp_log.h"
#include "esp_system.h"
#include "lvgl.h"

static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
#if DISP_DEBUG
static void display_monitor(lv_disp_drv_t *disp_drv, uint32_t time, uint32_t px);
#endif  // DISP_DEBUG

void
display_init(void)
{
	ESP_LOGI(TAG, "Initializing display");

	st7735s_init();

	ESP_LOGI(TAG, "Setting up LVGL");

	static lv_disp_buf_t disp_buf;
	static lv_color_t buf1[ST7735S_BUF_LEN];
	static lv_color_t buf2[ST7735S_BUF_LEN];
	lv_disp_buf_init(&disp_buf, buf1, buf2, ST7735S_BUF_CNT);

	lv_disp_drv_t disp;
	lv_disp_drv_init(&disp);
	disp.flush_cb = display_flush;
#if DISP_DEBUG
	disp.monitor_cb = display_monitor;
#endif  // DISP_DEBUG
	disp.buffer = &disp_buf;
	lv_disp_drv_register(&disp);

	ESP_LOGI(TAG, "Display initialized");
}

static void IRAM_ATTR
display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
	st7735s_draw(area->x1, area->x2, area->y1, area->y2, (uint16_t *)color_p);
	lv_disp_flush_ready(disp);
}

#if DISP_DEBUG
static void
display_monitor(lv_disp_drv_t *disp_drv, uint32_t time, uint32_t px)
{
	ESP_LOGI(TAG, "%d px refreshed in %d ms", px, time);
}
#endif  // DISP_DEBUG
