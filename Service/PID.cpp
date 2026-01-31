#include "PID.hpp"
#include "Encoder/encoder_service.hpp"

MotorPID::MotorPID(float kp, float kd, float ki,float &expected_speed)
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
    target_RPM_ = expected_speed;
}

void MotorPID::SetSpeedRPM(float rpm, bool cw)
{
    target_RPM_ = rpm;
    clock_wise_ = cw; = 0.0;
    error_sum_ = 0.0;
}

void MotorPID::SetSpeedRadPSec(float rps, bool cw)
{
    float xrps = (rps / (2.0* PI_value) );
    xrps *= 60.0;
    SetSpeedRPM(xrps, cw);
}

float MotorPID::PID(float &expected_RPM, float &actual_RPM,float &error, float &p, float &d, float &i)
{
    expected_RPM = target_RPM_
    actual_RPM = encoder_getRPM();
    error = expected_speed - actual_RPM;
    
    p = error * kp_;
    d = (error - last_error_) * kd_;
    i =  (error_sum_+ error)* ki_;
}

float MotorPID::DoPID()
{
    float expected_RPM,actual_RPM;
    float p,i,d;
    float error ;

    float pid_Process_result = this->PID(expected_RPM,actual_RPM,error,p,d,i);
}


/*virtual void MotorPID::HandleRotate(bool cw)
{
    Mptr->setMotor(cw,0.0);
}*/

MotorPID::~MotorPID()
{
    
}