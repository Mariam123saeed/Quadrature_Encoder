#include "Motor.hpp"

Motor::Motor(Motor_cfg *driver)
{
    gpio_init(driver->Pin1);
    gpio_init(driver->Pin2);
    gpio_set_dir(driver->Pin1, GPIO_OUT);
    gpio_set_dir(driver->Pin2, GPIO_OUT);

    gpio_set_function(driver->En_PIn, GPIO_FUNC_PWM);
    driver->_pwmSlice = pwm_gpio_to_slice_num(driver->En_PIn);
    pwm_set_wrap(driver->_pwmSlice, 1000);
    pwm_set_enabled(driver->_pwmSlice, true);
}

void Motor::Motor_Move_Right(Motor_cfg * driver,float duty)
{
    gpio_put(driver->Pin1, 1);
    gpio_put(driver->Pin2, 0);
    pwm_set_gpio_level(driver->En_PIn, duty * 1000);
}

void Motor::Motor_Move_Left(Motor_cfg *driver,float duty)
{
    gpio_put(driver->Pin1, 0);
    gpio_put(driver->Pin2, 1);
    pwm_set_gpio_level(driver->En_PIn, duty * 1000);
}

void Motor::Motor_stop(Motor_cfg * driver)
{
    gpio_put(driver->Pin1, 0);
    gpio_put(driver->Pin2, 0);
    pwm_set_gpio_level(driver->En_PIn, 0 * 1000);
}
