#ifndef __DEVICE_ST7735S__
#define __DEVICE_ST7735S__

#include <stdint.h>

#define ST7735S_WIDTH 132
#define ST7735S_HEIGHT 132

#define ST7735S_BUF_CNT (ST7735S_WIDTH * 8)
#define ST7735S_BUF_LEN (ST7735S_BUF_CNT * sizeof(uint16_t))

#define RGB565(rgb) \
	((((rgb >> 16) & 0x1f) << 0) | (((rgb >> 8) & 0x3f) << 5) | (((rgb >> 0) & 0x1f) << 11))

/**
 * @brief Initialize the ST7735S LCD display.
 */
void st7735s_init(void);

/**
 * @brief Draw a pixel on the display.
 *
 * @param x1 Start x coordinate.
 * @param x2 End x coordinate.
 * @param y1 Start y coordinate.
 * @param y2 End y coordinate.
 * @param pixels Pixel buffer.
 */
void st7735s_draw(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t *pixels);

/**
 * @brief Fill the entire screen with the specified color.
 *
 * @param x1 Start x coordinate.
 * @param x2 End x coordinate.
 * @param y1 Start y coordinate.
 * @param y2 End y coordinate.
 * @param color Color to fill with.
 */
void st7735s_fill(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t color);

#endif  // __DEVICE_ST7735S__
