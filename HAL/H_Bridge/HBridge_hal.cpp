#include "H_Bridge/HBridge_hal.hpp"

HBridge::HBridge(uint in1, uint in2, uint en)
    : _in1(in1), _in2(in2), _en(en) {}

void HBridge::init() {
    gpio_init(_in1);
    gpio_init(_in2);
    gpio_set_dir(_in1, GPIO_OUT);
    gpio_set_dir(_in2, GPIO_OUT);

    gpio_set_function(_en, GPIO_FUNC_PWM);
    _pwmSlice = pwm_gpio_to_slice_num(_en);

    pwm_set_wrap(_pwmSlice, 1000);
    pwm_set_enabled(_pwmSlice, true);
}

void HBridge::setMotor(MotorState state, float duty) {
    if (duty < 0) duty = 0;
    if (duty > 1) duty = 1;

    switch (state) {
        case MotorState::CW:
            gpio_put(_in1, 1);
            gpio_put(_in2, 0);
            break;

        case MotorState::CCW:
            gpio_put(_in1, 0);
            gpio_put(_in2, 1);
            break;

        case MotorState::STOP:
        default:
            gpio_put(_in1, 0);
            gpio_put(_in2, 0);
            duty = 0;
            break;
    }

    pwm_set_gpio_level(_en, duty * 1000);
}