#include <stdio.h>
#include "pico/stdlib.h"

/* Encoder HAL + Service */

#include "Encoder/encoder_hal.hpp"
#include "Encoder/encoder_config.hpp"
#include "Encoder/encoder_service.hpp"
#include "Motor/Motor.hpp"
#include "PID/PID.hpp"


Motor::Motor_cfg Motor1
{
   .En_PIn = 6,
   .Pin1 = 2,
   .Pin2 = 3,
   ._pwmSlice = 6,
};

Motor::Motor_cfg Motor2
{
   .En_PIn = 7,
   .Pin1 = 4,
   .Pin2 = 5,
   ._pwmSlice = 7,
};


int main() 
{
    stdio_init_all();
    sleep_ms(2000);  // wait for Serial monitor

    printf("Starting Motor + Dual Encoder Closed-loop Test...\n");

    /* ---------------------------
       Encoder initialization
    --------------------------- */
    EncoderHAL encoder1(ENCODER1_PIN_A, ENCODER1_PIN_B);
    EncoderHAL encoder2(ENCODER2_PIN_A, ENCODER2_PIN_B);
    encoder1.encoder_init();
    encoder2.encoder_init();

    EncoderService service1(encoder1);
    EncoderService service2(encoder2);
    service1.encoder_start();
    service2.encoder_start();

    /* ---------------------------
       Motor initialization
    --------------------------- */
   Motor MotorA(&Motor1);
   Motor MotorB(&Motor2);


   float targetRPM = 120.0f; // Desired motor speed

//   float Kp = 0.007f;        // Simple proportional gain (tune experimentally)
   //float motorOutput = 0.0f;

   MotorPID::PIDINPUT pid_input = 
   {
    .kp = 0.4f,
    .kd = 0.01f,
    .ki = 0.02f,
    .dt = 0.1f,
    .expected_speed = targetRPM,
   };

   MotorPID pid(&pid_input);
   pid.SetSpeedRPM(targetRPM, true);

   while (true) {
        /* ---------------------------
           Read encoder RPM
        --------------------------- */
        float currentRPM = service1.encoder_getRPM();

        /* ---------------------------
           Simple proportional control
        --------------------------- */
        //float error = targetRPM - currentRPM;
        //motorOutput += Kp * error;

        /*// Limit output to [-1.0, 1.0]
        if (motorOutput > 1.0f) motorOutput = 1.0f;
        if (motorOutput < -1.0f) motorOutput = -1.0f;
         */

        /************************* PID Part ******************* */
        float throttle =  pid.UpdateThrottle(currentRPM);
        /* ---------------------------
           Set motor speed
        --------------------------- */
        MotorA.Motor_Move_Right(&Motor1,throttle);
        MotorB.Motor_Move_Right(&Motor2,throttle);
        sleep_ms(10000); 
        MotorA.Motor_Move_Left(&Motor1,throttle);
        MotorB.Motor_Move_Left(&Motor2,throttle);
        sleep_ms(10000);
        MotorA.Motor_stop(&Motor1);
        MotorB.Motor_stop(&Motor2);
        sleep_ms(10000);



        /* ---------------------------
           Read and print encoder data
        --------------------------- */
        int32_t ticks1 = encoder1.encoder_getTicks();
        float rpm1   = service1.encoder_getRPM();
        float speed1 = service1.encoder_getSpeedCmS();
        float dist1  = service1.encoder_getDistanceCm();
        float rot1   = service1.encoder_getRotations();

        int32_t ticks2 = encoder2.encoder_getTicks();
        float rpm2   = service2.encoder_getRPM();
        float speed2 = service2.encoder_getSpeedCmS();
        float dist2  = service2.encoder_getDistanceCm();
        float rot2   = service2.encoder_getRotations();


        printf("Encoder1 | Ticks: %d | RPM: %.2f | Speed: %.2f cm/s | Distance: %.2f cm | Rotations: %.2f\n",
               ticks1, rpm1, speed1, dist1, rot1);

        printf("Encoder2 | Ticks: %d | RPM: %.2f | Speed: %.2f cm/s | Distance: %.2f cm | Rotations: %.2f\n\n",
               ticks2, rpm2, speed2, dist2, rot2);

        sleep_ms(100); // Loop delay
    }

    return 0;
}
