#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "H_Bridge/HBridge_hal.hpp"

class Motor {
public:
    Motor(HBridge& driver);

    void init();
    void setSpeed(float speed); // -1.0 >> +1.0
    void stop();

private:
    HBridge& _driver;
};

#endif
