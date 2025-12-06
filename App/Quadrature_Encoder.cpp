#include <stdio.h>
#include "pico/stdlib.h"
#include "encoder_hal.hpp"
#include "encoder_service.hpp"
 
/***************************************************************
 * Function: main
 * Layer: Application Layer
 * Description:
 *     - Initializes stdio (UART/USB) for debugging output.
 *     - Creates and initializes EncoderHAL object.
 *     - Creates EncoderService object and starts periodic updates.
 *     - Infinite loop prints all metrics every 500ms.
 *     - Demonstrates clean separation of concerns:
 *         HAL handles hardware
 *         Service handles computation
 *         Main handles application-level control
 ****************************************************************/
int main() {
    stdio_init_all();  // Initialize UART/USB for printf output

    EncoderHAL encoder;
    encoder.encoder_init();  // Initialize GPIO and interrupts

    EncoderService service(encoder);
    service.encoder_start(); // Start periodic metric calculations

    while(true) {
        printf("Ticks: %d, RPM: %.2f, Speed: %.2f cm/s, Distance: %.2f cm, Rotations: %.2f\n",
               encoder.encoder_getTicks(),
               service.encoder_getRPM(),
               service.encoder_getSpeedCmS(),
               service.encoder_getDistanceCm(),
               service.encoder_getRotations());
        sleep_ms(500);
    }
}
