/***************************************************************
 *  File: encoder_hal.h
 *  Description:
 *      - Header file for encoder HAL module.
 *      - Defines types and functions available to higher layers.
 ****************************************************************/

#ifndef ENCODER_HAL_H
#define ENCODER_HAL_H

#include "pico/stdlib.h"
#include <stdbool.h>

// Encoder rotation directions
typedef enum {
    DIR_FORWARD,   // Forward rotation
    DIR_BACKWARD,  // Backward rotation
    DIR_UNKNOWN    // Unknown or not moving
} encoder_direction_t;

// Initialize encoder pins
void encoder_init(uint pin_a, uint pin_b);
// Get the number of encoder ticks
int32_t encoder_get_ticks();
// Reset tick count to zero
void encoder_clear();
// Get the current rotation direction
encoder_direction_t encoder_get_direction();

#endif // ENCODER_HAL_H
