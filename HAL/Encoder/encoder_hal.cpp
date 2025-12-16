/***************************************************************
 *  File: encoder_hal.cpp
 *  Layer: HAL (Hardware Abstraction Layer)
 *  Description:
 *      - Handles low-level hardware interaction with the quadrature encoder
 *        connected to Raspberry Pi Pico GPIO pins.
 *      - Reads channels A & B, processes GPIO interrupts, determines
 *        rotation direction, and counts encoder ticks.
 *      - Encapsulates all hardware-specific details behind a class interface.
 ****************************************************************/

#include "Encoder/encoder_hal.hpp"

// ---------------------------
// Static singleton-like instance
// ---------------------------
// Used to allow ISR (Interrupt Service Routine) access to the object.
// Since ISRs cannot have a 'this' pointer, a static object pointer is required.
EncoderHAL* EncoderHAL::instance = nullptr;

// ---------------------------
// Constructor
// ---------------------------
EncoderHAL::EncoderHAL()
    : _pinA(ENCODER_PIN_A), _pinB(ENCODER_PIN_B),
      _ticks(0), _direction(EncoderDirection::UNKNOWN),
      _lastA(false), _lastB(false)
{
    // Assign this object to static instance for ISR callback access
    instance = this;
     
}

// ---------------------------
// Method: encoder_init
// ---------------------------
// Description:
//     - Initializes the encoder GPIO pins as inputs with pull-up resistors.
//     - Reads initial states of pins A and B.
//     - Configures interrupts on both rising and falling edges for accurate counting.
// Notes:
//     - _pinA interrupt callback is set to a static method, which delegates
//       processing to the object via the static instance pointer.
//     - _pinB interrupt does not need a callback because _pinA's ISR handles both channels.
void EncoderHAL::encoder_init() {
    gpio_init(_pinA);
    gpio_init(_pinB);
    gpio_set_dir(_pinA, GPIO_IN);
    gpio_set_dir(_pinB, GPIO_IN);
    gpio_pull_up(_pinA);
    gpio_pull_up(_pinB);

    _lastA = gpio_get(_pinA);
    _lastB = gpio_get(_pinB);

    // Attach interrupts
    gpio_set_irq_enabled_with_callback(_pinA, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &EncoderHAL::encoder_gpioCallback);
    gpio_set_irq_enabled(_pinB, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}

// ---------------------------
// Method: encoder_getTicks
// ---------------------------
// Returns the current encoder tick count.
// Marked const because it does not modify object state.
int32_t EncoderHAL::encoder_getTicks() const { return _ticks; }

// ---------------------------
// Method: encoder_clear
// ---------------------------
// Resets the tick counter and rotation direction to initial state.
void EncoderHAL::encoder_clear() { _ticks = 0; _direction = EncoderDirection::UNKNOWN; }

// ---------------------------
// Method: encoder_getDirection
// ---------------------------
// Returns the last detected rotation direction.
// Marked const because it does not modify object state.
EncoderDirection EncoderHAL::encoder_getDirection() const { return _direction; }

// ---------------------------
// Static ISR Callback
// ---------------------------
// Description:
//     - GPIO interrupt handler called by hardware on pin state change.
//     - Delegates the actual handling to the object's handleEncoder method
//       using the static 'instance' pointer.
void EncoderHAL::encoder_gpioCallback(uint gpio, uint32_t events) {
    if(instance) {
        instance->handleEncoder();
    }
}

// ---------------------------
// Method: handleEncoder
// ---------------------------
// Description:
//     - Reads the current states of encoder pins A and B.
//     - Determines rotation direction and increments/decrements tick count.
//     - Implements standard quadrature encoder logic:
//           - Detects forward/backward based on previous and current states.
//     - Updates internal last states (_lastA, _lastB) for next interrupt.
void EncoderHAL::handleEncoder() {
    bool a = gpio_get(_pinA);
    bool b = gpio_get(_pinB);

    if (a != _lastA || b != _lastB) 
    {
        if (_lastA == _lastB) {
            if (a != b) { _ticks++; _direction = EncoderDirection::FORWARD; }
            else { _ticks--; _direction = EncoderDirection::BACKWARD; }
        } else {
            if (a == b) { _ticks++; _direction = EncoderDirection::FORWARD; }
            else { _ticks--; _direction = EncoderDirection::BACKWARD; }
        }
    }

    // Store last pin states for next comparison
    _lastA = a;
    _lastB = b;
}
