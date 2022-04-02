#define TAG "UI"

#include "esp_log.h"
#include "esp_system.h"
#include "lvgl.h"
#include "tasks.h"

#define ANIMATION 0

void
task_ui(void *arg)
{
#if ANIMATION
	int v1 = 0, v2 = 0, v3 = 0;
	int d1 = 1, d2 = 2, d3 = 3;
#endif

	static lv_color_t needle_colors[3];
	needle_colors[0] = LV_COLOR_RED;
	needle_colors[1] = LV_COLOR_GREEN;
	needle_colors[2] = LV_COLOR_BLUE;

	lv_obj_t *gauge1 = lv_gauge_create(lv_scr_act(), NULL);
	lv_gauge_set_needle_count(gauge1, 3, needle_colors);
	lv_obj_set_pos(gauge1, 10, 10);
	lv_obj_set_size(gauge1, 108, 108);

#if ANIMATION
	while (1) {
		lv_gauge_set_value(gauge1, 0, v1);
		lv_gauge_set_value(gauge1, 1, v2);
		lv_gauge_set_value(gauge1, 2, v3);

		if (v1 + d1 > 100 || v1 + d1 < 0) d1 = -d1;
		if (v2 + d2 > 100 || v2 + d2 < 0) d2 = -d2;
		if (v3 + d3 > 100 || v3 + d3 < 0) d3 = -d3;
		v1 += d1;
		v2 += d2;
		v3 += d3;

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
#else
	lv_gauge_set_value(gauge1, 0, 0);
	lv_gauge_set_value(gauge1, 1, 20);
	lv_gauge_set_value(gauge1, 2, 40);
#endif  // ANIMATION

	vTaskDelete(NULL);
}
