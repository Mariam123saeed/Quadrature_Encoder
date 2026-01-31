#ifndef PID_HPP_
#define _PID_HPP_
/******************************************* Include Part ******************************************** */
#include <stdint.h>
#define PI_value 3.14159

class MotorPID
{
    public:
    //MotorState &Mptr;
        MotorPID(float kp, float kd, float ki,float,float &expected_speed);
        virtual ~MotorPID();

        void SetSpeedRadPSec(float rps, bool cw);
        void SetSpeedRPM(float rpm, bool cw);
        
        /***
        * Use PID to update the throttle
        * @return current error level between set actual and expected value
        */
        float DoPID();

        /***
         * Do the PIC calculate and return the component parts.
         * @param actual_speed : the speed that is already output to the motor
         * @param error: difference between expected_speed and actual_speed 
         * @param p: p = kP * err
         * @param i : xCumErr += err; then i = kI * xCumErr;
         * @param d : d = kD * (err - xLastErr);
         * @return : the Actual Speed will apply by PWM to the motor
         */
        float PID(float &actual_RPM,float &error, float &p, float &d, float &i);

    protected:
        virtual void HandleRotate(bool cw);
        float target_RPM_ = 0.0;
        float output_RPM_;
        float clock_wise_;

    private:
    float prev_error_;
    float kp_;
    float ki_;
    float kd_;
    float last_error_;
    float error_sum_;
};

#endif  /*_PID_HPP_*/