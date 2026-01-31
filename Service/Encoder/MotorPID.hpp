#ifndef _PID_HPP_
#define _PID_HPP_
/**
 * @file pid.hpp
 * @brief Simple PID controller implementation dedicated for DC motor speed control
 *
 * This class implements a classic discrete PID controller with anti-windup (clamped integral)
 * mainly intended for DC motor velocity regulation using PWM output.
 */

/******************************************* Include Part ******************************************** */
#include <stdint.h>
#define PI_value 3.14159        ///< Mathematical constant π (float version)
#define MAX_RPM 210.0f          ///< Maximum motor speed used as integral windup limit

/**
 * @class MotorPID
 * @brief PID controller for single DC motor velocity control
 *
 * Features:
 * - velocity setpoint in RPM or rad/s
 * - direction control (CW/CCW)
 * - anti-windup via integral clamping
 * - separate access to P, I, D components for debugging/tuning
 */
class MotorPID
{
    public:

    /**
     * @struct PIDINPUT
     * @brief Input configuration structure for PID controller
     */
    typedef struct 
    {
        float kp;               ///< Proportional gain
        float kd;               ///< Integral gain
        float ki;               ///< Derivative gain
        float expected_speed;   ///< Target speed [RPM]
        float dt;               ///< Time step / sample time [seconds]
    }PIDINPUT;

    /**
     * @struct PIDOutput
     * @brief Detailed PID calculation result (components + total)
     */
    typedef struct 
    {
        float p;                ///< Proportional term
        float i;                ///< Integral term (after clamping)
        float d;                ///< Derivative term
        float total;            ///< Final PID output (controller effort)
    }PIDOutput;

        /**
        * @brief Constructor
        * @param PIDIn pointer to PID tuning parameters and initial setpoint
        */
        MotorPID(PIDINPUT * PIDIn);

        /**
         * @brief Virtual destructor
        */
        virtual ~MotorPID();

        /**
         * @brief Set desired motor speed in radians per second
         * @param rps target angular velocity [rad/s]
         * @param cw rotation direction (true = clockwise)
        */
        void SetSpeedRadPSec(float rps, bool cw);

        /**
         * @brief Set desired motor speed in revolutions per minute
         * @param rpm target speed [RPM]
         * @param cw rotation direction (true = clockwise)
        */
        void SetSpeedRPM(float rpm, bool cw);
        
        /**
        * @brief Full PID calculation – returns all components separately
        * @param actual_RPM current measured motor speed [RPM]
        * @return structure containing P, I, D terms and total output
        *
        * Calculation steps performed:
        *   error = setpoint − actual
        *   P     = Kp × error
        *   I     = Ki × (∫error)   (with anti-windup clamping)
        *   D     = Kd × (error − prev_error)/dt
        *   output = P + I + D
        */
        PIDOutput PID(float actual_RPM);

        /**
         * @brief Perform one PID control step and return current error
         * @param actual_RPM measured/estimated current motor speed [RPM]
         * @return current control error = target_RPM - actual_RPM
        */
        float DoPID( float actual_RPM);


    private:


        /**
         * @brief Clamp value to specified range
         * @param value input value
         * @param min lower bound
         * @param max upper bound
         * @return clamped value
         */
        inline float clamp(float value, float min, float max);


        float target_RPM_ ;              ///< Current speed setpoint [RPM]
        bool clock_wise_;                ///< Rotation direction flag
        float throttle_;                ///< Current controller output (typically PWM duty)
        float dt_;                    ///< Sample time [s]
        float kp_;                   ///< Proportional gain
        float ki_;                  ///< Integral gain
        float kd_;                 ///< Derivative gain
        float error_;             ///< Current error
        float last_error_;       ///< Previous error (used for derivative)
        float error_sum_;       ///< Accumulated error (integral part before clamping)
        float integral_max_;   ///< Upper limit for integral sum (anti-windup)
        float integral_min_;  ///< Lower limit for integral sum (anti-windup)

};

#endif  /*_PID_HPP_*/
