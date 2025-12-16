#ifndef ENCODER_HAL_HPP
#define ENCODER_HAL_HPP

#include "pico/stdlib.h"
#include "Encoder/encoder_config.hpp"

// Enum representing rotation direction of the encoder
enum class EncoderDirection { UNKNOWN, FORWARD, BACKWARD };

/***************************************************************
 * Class: EncoderHAL
 * Layer: HAL (Hardware Abstraction Layer)
 * Description:
 *     - Provides low-level interface for interacting with a quadrature encoder
 *       connected to Raspberry Pi Pico GPIO pins.
 *     - Handles pin initialization, GPIO interrupts, and tick counting.
 *     - Implements singleton-like pointer to allow ISR access to object.
 ****************************************************************/

class EncoderHAL {
public:
    /***********************************************************
     * Constructor: EncoderHAL
     * Description:
     *     - Initializes pin numbers, tick counter, direction,
     *       and last pin states.
     *     - Sets the static instance pointer for ISR access.
     ***********************************************************/
    EncoderHAL();

    /***********************************************************
     * Method: encoder_init
     * Description:
     *     - Initializes GPIO pins for the encoder channels.
     *     - Configures pull-ups and attaches interrupts.
     *     - Reads initial states of A and B channels.
     ***********************************************************/
    void encoder_init();

    /***********************************************************
     * Method: encoder_getTicks
     * Description:
     *     - Returns the current number of encoder ticks counted.
     *     - Marked const to indicate it does not modify object state.
     ***********************************************************/
    int32_t encoder_getTicks() const;

    /***********************************************************
     * Method: encoder_clear
     * Description:
     *     - Resets tick counter to zero.
     *     - Resets direction to UNKNOWN.
     ***********************************************************/
    void encoder_clear();

    /***********************************************************
     * Method: encoder_getDirection
     * Description:
     *     - Returns the last detected rotation direction.
     *     - Marked const to indicate it does not modify object state.
     ***********************************************************/
    EncoderDirection encoder_getDirection() const;

private:
    /***********************************************************
     * Static ISR Callback: encoder_gpioCallback
     * Description:
     *     - Static ISR callback called by GPIO interrupt.
     *     - Delegates processing to the object's handleEncoder method
     *       via singleton instance pointer.
     ***********************************************************/
    static void encoder_gpioCallback(uint gpio, uint32_t events);

    /***********************************************************
     * Method: handleEncoder
     * Description:
     *     - Processes the quadrature signals to detect direction.
     *     - Updates tick count and last states of pins.
     *     - Called both by ISR and internally if needed.
     ***********************************************************/
    void handleEncoder();

    uint _pinA, _pinB;                      // Encoder GPIO pins
    volatile int32_t _ticks;                // Tick counter (volatile for ISR safety)
    volatile EncoderDirection _direction;   // Last detected direction
    volatile bool _lastA, _lastB;           // Last states of pins

    static EncoderHAL* instance;            // Singleton-like pointer for ISR access
};


#endif
