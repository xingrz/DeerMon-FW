#ifndef __DEVICE_BACKLIGHT__
#define __DEVICE_BACKLIGHT__

#include <stdint.h>

/**
 * @brief Initialize the backlight.
 */
void bl_init(void);

/**
 * @brief Set the backlight level.
 *
 * @param level Backlight level from 0 (off) to 16 (brightest).
 */
void bl_set(uint8_t level);

#endif  // __DEVICE_BACKLIGHT__
