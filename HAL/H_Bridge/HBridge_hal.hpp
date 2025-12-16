#ifndef HBRIDGE_HAL_HPP
#define HBRIDGE_HAL_HPP

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "H_Bridge/HBridge_config.hpp"

// Motor direction
enum class MotorState {
    STOP,
    CW,
    CCW
};

class HBridge {
public:
    HBridge(uint in1, uint in2, uint en);
    void init();
    void setMotor(MotorState state, float duty); // duty: 0.0 -> 1.0

private:
    uint _in1, _in2, _en;
    uint _pwmSlice;
};

#endif