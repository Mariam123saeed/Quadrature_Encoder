#include <stdio.h>
#include "pico/stdlib.h"
#include "Encoder/encoder_hal.hpp"
#include "H_Bridge/HBridge_hal.hpp"
#include "Encoder/encoder_service.hpp"

int main() {
    stdio_init_all();

    EncoderHAL encoder;
    encoder.encoder_init();
    EncoderService service(encoder);
    service.encoder_start();

    // Define HBridge motor (pins GPIO 2,3 direction, GPIO 4 PWM)
    HBridge motorA(2, 3, 4);
    motorA.init();

    // Example: run motor at 80% speed forward
    motorA.setMotor(MotorState::CW, 0.8f);

    while(true) {
        printf("Ticks: %d, RPM: %.2f, Speed: %.2f cm/s, Distance: %.2f cm, Rotations: %.2f\n",
               encoder.encoder_getTicks(),
               service.encoder_getRPM(),
               service.encoder_getSpeedCmS(),
               service.encoder_getDistanceCm(),
               service.encoder_getRotations());

        // Example feedback control
        float targetRPM = 100.0f;
        float currentRPM = service.encoder_getRPM();
        if(currentRPM < targetRPM) motorA.setMotor(MotorState::CW, 0.9f);
        else if(currentRPM > targetRPM) motorA.setMotor(MotorState::CW, 0.7f);

        sleep_ms(100);
    }
}