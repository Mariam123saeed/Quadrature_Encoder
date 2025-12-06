#include <stdio.h>
#include "pico/stdlib.h"
#include "encoder_hal.hpp"
#include "encoder_service.hpp"
 
int main() {
    stdio_init_all();

    EncoderHAL encoder;
    encoder.init();

    EncoderService service(encoder);
    service.start();

    while(true) {
        printf("Ticks: %d, RPM: %.2f, Speed: %.2f cm/s, Distance: %.2f cm, Rotations: %.2f\n",
               encoder.getTicks(),
               service.getRPM(),
               service.getSpeedCmS(),
               service.getDistanceCm(),
               service.getRotations());
        sleep_ms(500);
    }
}
