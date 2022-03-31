#ifndef __DEVICE_BUZZER__
#define __DEVICE_BUZZER__

#include <stdint.h>

/**
 * @brief Initialize the buzzer.
 */
void buzzer_init(void);

/**
 * @brief Set the buzzer level.
 *
 * @param level Buzzer level from 0 to 7.
 */
void buzzer_set(uint8_t level);

#endif  // __DEVICE_BUZZER__
