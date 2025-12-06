/***************************************************************
 *  File: encoder_hal.hpp
 *  Description:
 *      - Header file for encoder HAL module.
 *      - Defines types and functions available to higher layers.
 ****************************************************************/

#ifndef ENCODER_HAL_HPP
#define ENCODER_HAL_HPP

#include "pico/stdlib.h"
#include "encoder_config.hpp"

enum class EncoderDirection { UNKNOWN, FORWARD, BACKWARD };

class EncoderHAL {
public:
    EncoderHAL();
    void init();
    int32_t getTicks() const;
    void clear();
    EncoderDirection getDirection() const;

private:
    static void gpioCallback(uint gpio, uint32_t events);
    void handleEncoder();

    uint _pinA, _pinB;
    volatile int32_t _ticks;
    volatile EncoderDirection _direction;
    volatile bool _lastA;
    volatile bool _lastB;

    static EncoderHAL* instance; // For ISR callback
};

#endif
