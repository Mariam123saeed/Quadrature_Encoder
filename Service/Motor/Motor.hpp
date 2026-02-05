#include "pico/stdlib.h"
#include "hardware/pwm.h"

class Motor 
{
public:
    enum class MotorState { STOP, CW, CCW };

    struct Motor_cfg {
        unsigned int  En_PIn;
        unsigned int  Pin1;
        unsigned int  Pin2;
        unsigned int  _pwmSlice;
    };

    Motor(Motor_cfg * driver);
    void Motor_Move_Right(Motor_cfg *driver, float duty);
    void Motor_Move_Left(Motor_cfg * driver, float duty);
    void Motor_stop(Motor_cfg * driver);
};
