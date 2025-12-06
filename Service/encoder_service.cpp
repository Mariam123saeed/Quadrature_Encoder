/***************************************************************
 *  File: encoder_service.cpp
 *  Layer: Service Layer
 *  Description:
 *      - Converts raw encoder ticks into meaningful physical
 *        values: RPM, speed, distance, rotations.
 *      - Runs a periodic timer every 100ms to update values.
 ****************************************************************/

#include "encoder_service.hpp"
#include <cmath>

EncoderService::EncoderService(EncoderHAL& encoder)
    : _encoder(encoder),
      _lastTicks(0), _currentTicks(0),
      _rpm(0), _speedCmS(0), _distanceCm(0)
{}

void EncoderService::start() {
    add_repeating_timer_ms(100, EncoderService::timerCallback, this, &_timer);
}

bool EncoderService::timerCallback(struct repeating_timer* t) {
    EncoderService* service = static_cast<EncoderService*>(t->user_data);
    service->update();
    return true;
}

void EncoderService::update() {
    _currentTicks = _encoder.getTicks();
    int32_t delta = _currentTicks - _lastTicks;

    // RPM calculation
    _rpm = (delta * 600.0f) / ENCODER_CPR;

    // Distance calculation
    _distanceCm += (delta / static_cast<float>(ENCODER_CPR)) * 2.0f * 3.1415926f * WHEEL_RADIUS_CM;

    // Linear speed (cm/s) over 100ms interval
    _speedCmS = (delta / static_cast<float>(ENCODER_CPR)) * 2.0f * 3.1415926f * WHEEL_RADIUS_CM / 0.1f;

    _lastTicks = _currentTicks;
}

float EncoderService::getRPM() const { return _rpm; }
float EncoderService::getSpeedCmS() const { return _speedCmS; }
float EncoderService::getDistanceCm() const { return _distanceCm; }
float EncoderService::getRotations() const { return _currentTicks / static_cast<float>(ENCODER_CPR); }
