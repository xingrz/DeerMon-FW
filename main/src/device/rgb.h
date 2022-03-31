#ifndef __DEVICE_RGB__
#define __DEVICE_RGB__

#include <stdint.h>

#define RGB(r, g, b) (((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF))

#define GET_R(rgb) ((rgb & 0xFF0000) >> 16)
#define GET_G(rgb) ((rgb & 0x00FF00) >> 8)
#define GET_B(rgb) ((rgb & 0x0000FF))

/**
 * @brief Initialize the RGB LED.
 */
void rgb_init(void);

/**
 * @brief Set the RGB LED to the given color.
 *
 * @param color RGB888 color.
 */
void rgb_set(uint32_t color);

#endif  // __DEVICE_RGB__
