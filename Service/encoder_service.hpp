/***************************************************************
 *  File: encoder_service.hpp
 *  Description:
 *      - Header file for encoder service layer.
 *      - Provides high-level APIs for reading RPM, speed, etc.
 ****************************************************************/
#ifndef ENCODER_SERVICE_HPP
#define ENCODER_SERVICE_HPP

#include "encoder_hal.hpp"
#include "hardware/timer.h"

class EncoderService {
public:
    EncoderService(EncoderHAL& encoder);
    void start();
    float getRPM() const;
    float getSpeedCmS() const;
    float getDistanceCm() const;
    float getRotations() const;

private:
    static bool timerCallback(struct repeating_timer* t);
    void update();

    EncoderHAL& _encoder;
    int32_t _lastTicks;
    int32_t _currentTicks;
    float _rpm;
    float _speedCmS;
    float _distanceCm;
    struct repeating_timer _timer;
};

#endif
