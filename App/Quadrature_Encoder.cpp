#include <stdio.h>
#include "pico/stdlib.h"

/* Encoder HAL + Service */
#include "Encoder/encoder_hal.hpp"
#include "Encoder/encoder_service.hpp"

/* Motor */
#include "Motor/Motor.hpp"
#include "H_Bridge/HBridge_hal.hpp"

/* ---------------------------
   Pins configuration
--------------------------- */
#define MOTOR_A_PIN1 2
#define MOTOR_A_PIN2 3
#define MOTOR_A_PWM  4

int main() {
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
    HBridge hbridgeA(MOTOR_A_PIN1, MOTOR_A_PIN2, MOTOR_A_PWM);
    Motor motorA(hbridgeA);
    motorA.init();

    /* ---------------------------
       Closed-loop variables
    --------------------------- */
    float targetRPM = 120.0f; // Desired motor speed
    float Kp = 0.007f;        // Simple proportional gain (tune experimentally)
    float motorOutput = 0.0f;

    while (true) {
        /* ---------------------------
           Read encoder RPM
        --------------------------- */
        float currentRPM = service1.encoder_getRPM();

        /* ---------------------------
           Simple proportional control
        --------------------------- */
        float error = targetRPM - currentRPM;
        motorOutput += Kp * error;

        // Limit output to [-1.0, 1.0]
        if (motorOutput > 1.0f) motorOutput = 1.0f;
        if (motorOutput < -1.0f) motorOutput = -1.0f;

        /* ---------------------------
           Set motor speed
        --------------------------- */
        motorA.setSpeed(motorOutput);

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
