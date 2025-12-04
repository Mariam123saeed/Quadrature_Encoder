/***************************************************************
 *  File: encoder_hal.c
 *  Layer: HAL (Hardware Abstraction Layer)
 *  Description:
 *      - This file handles low-level interaction with the
 *        quadrature encoder hardware connected to the 
 *        Raspberry Pi Pico GPIO pins.
 *      - It reads channels A & B, processes interrupts,
 *        determines rotation direction, and counts ticks.
 ****************************************************************/
#include "encoder_hal.h"

static uint _pin_a, _pin_b;              // Encoder GPIO pins
static volatile int32_t _ticks = 0;      // Tick counter
static volatile encoder_direction_t _direction = DIR_UNKNOWN;  // Current rotation direction
// Last read pin states
static volatile bool _last_a = 0;         
static volatile bool _last_b = 0;


/***************************************************************
 * Function: encoder_callback
 * Description:
 *      - Interrupt Service Routine (ISR) triggered whenever
 *        channel A or B changes (rising or falling edge).
 *      - Reads both channels, applies quadrature logic,
 *        updates tick count and direction.
 * Params:
 *      gpio   : The pin that triggered the interrupt
 *      events : Type of edge (rise/fall)
 ***************************************************************/
void encoder_callback(uint gpio, uint32_t events) {
    bool a = gpio_get(_pin_a);
    bool b = gpio_get(_pin_b);
     
    // Check for state change
    if (a != _last_a || b != _last_b) {
        // Determine direction based on quadrature logic
        if (_last_a == _last_b) {
            if (a != b) {
                _ticks++;
                _direction = DIR_FORWARD;
            } else {
                _ticks--;
                _direction = DIR_BACKWARD;
            }
        } else {
            if (a == b) {
                _ticks++;
                _direction = DIR_FORWARD;
            } else {
                _ticks--;
                _direction = DIR_BACKWARD;
            }
        }
    }

    _last_a = a;
    _last_b = b;
}

/***************************************************************
 * Function: encoder_init
 * Description:
 *      - Initializes encoder pins as inputs.
 *      - Enables pull-ups.
 *      - Reads initial state.
 *      - Attaches interrupt callback for A & B edges.
 * Params:
 *      pin_a : GPIO number for channel A
 *      pin_b : GPIO number for channel B
 ***************************************************************/
void encoder_init(uint pin_a, uint pin_b) {
    _pin_a = pin_a;
    _pin_b = pin_b;

    gpio_init(_pin_a);
    gpio_init(_pin_b);

    gpio_set_dir(_pin_a, GPIO_IN);
    gpio_set_dir(_pin_b, GPIO_IN);

    gpio_pull_up(_pin_a);
    gpio_pull_up(_pin_b);
     
    // Save initial states
    _last_a = gpio_get(_pin_a);
    _last_b = gpio_get(_pin_b);

    // Enable IRQ on rising and falling edges
    gpio_set_irq_enabled_with_callback(_pin_a, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_callback);
    gpio_set_irq_enabled(_pin_b, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}

/***************************************************************
 * Function: encoder_get_ticks
 * Description:
 *      - Returns the total number of recorded encoder ticks.
 ***************************************************************/
int32_t encoder_get_ticks() {
    return _ticks;
}

/***************************************************************
 * Function: encoder_clear
 * Description:
 *      - Resets tick counter and direction.
 ***************************************************************/
void encoder_clear() {
    _ticks = 0;
    _direction = DIR_UNKNOWN;
}

/***************************************************************
 * Function: encoder_get_direction
 * Description:
 *      - Returns the last detected rotation direction.
 ***************************************************************/
encoder_direction_t encoder_get_direction() {
    return _direction;
}
