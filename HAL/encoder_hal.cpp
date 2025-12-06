/***************************************************************
 *  File: encoder_hal.cpp
 *  Layer: HAL (Hardware Abstraction Layer)
 *  Description:
 *      - This file handles low-level interaction with the
 *        quadrature encoder hardware connected to the 
 *        Raspberry Pi Pico GPIO pins.
 *      - It reads channels A & B, processes interrupts,
 *        determines rotation direction, and counts ticks.
 ****************************************************************/
#include "encoder_hal.hpp"

EncoderHAL* EncoderHAL::instance = nullptr;

EncoderHAL::EncoderHAL()
    : _pinA(ENCODER_PIN_A), _pinB(ENCODER_PIN_B),
      _ticks(0), _direction(EncoderDirection::UNKNOWN),
      _lastA(false), _lastB(false)
{
    instance = this;
}

void EncoderHAL::init() {
    gpio_init(_pinA);
    gpio_init(_pinB);
    gpio_set_dir(_pinA, GPIO_IN);
    gpio_set_dir(_pinB, GPIO_IN);
    gpio_pull_up(_pinA);
    gpio_pull_up(_pinB);

    _lastA = gpio_get(_pinA);
    _lastB = gpio_get(_pinB);

    gpio_set_irq_enabled_with_callback(_pinA, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &EncoderHAL::gpioCallback);
    gpio_set_irq_enabled(_pinB, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}

int32_t EncoderHAL::getTicks() const { return _ticks; }
void EncoderHAL::clear() { _ticks = 0; _direction = EncoderDirection::UNKNOWN; }
EncoderDirection EncoderHAL::getDirection() const { return _direction; }

void EncoderHAL::gpioCallback(uint gpio, uint32_t events) {
    if(instance) instance->handleEncoder();
}

void EncoderHAL::handleEncoder() {
    bool a = gpio_get(_pinA);
    bool b = gpio_get(_pinB);

    if (a != _lastA || b != _lastB) {
        if (_lastA == _lastB) {
            if (a != b) { _ticks++; _direction = EncoderDirection::FORWARD; }
            else { _ticks--; _direction = EncoderDirection::BACKWARD; }
        } else {
            if (a == b) { _ticks++; _direction = EncoderDirection::FORWARD; }
            else { _ticks--; _direction = EncoderDirection::BACKWARD; }
        }
    }
    _lastA = a;
    _lastB = b;
}
