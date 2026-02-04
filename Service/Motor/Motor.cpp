#include "Motor.hpp"

Motor::Motor(HBridge& driver)
    : _driver(driver) {}

void Motor::init() {
    _driver.init();
}

void Motor::setSpeed(float speed) {
    if (speed > 1.0f)  speed = 1.0f;
    if (speed < -1.0f) speed = -1.0f;

    if (speed > 0) {
        _driver.setMotor(MotorState::CW, speed);
    } 
    else if (speed < 0) {
        _driver.setMotor(MotorState::CCW, -speed);
    } 
    else {
        stop();
    }
}

void Motor::stop() {
    _driver.setMotor(MotorState::STOP, 0);
}
