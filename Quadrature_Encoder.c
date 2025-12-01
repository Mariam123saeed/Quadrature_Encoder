#include <stdio.h>
#include "pico/stdlib.h"
#include "encoder_service.h"

// ---- Encoder configuration ----
#define ENCODER_PIN_A 10          // GPIO pin connected to encoder channel A
#define ENCODER_PIN_B 11          // GPIO pin connected to encoder channel B
#define CPR 330                   // Counts per revolution (example: gear ratio 1:30)
#define WHEEL_RADIUS_CM 3.0f      // Wheel radius in cm

int main() 
{
    // Initialize UART/USB for printf output
    stdio_init_all();
    // Initialize encoder service layer (sets up HAL, timer, calculations)
    encoder_service_init(ENCODER_PIN_A, ENCODER_PIN_B, CPR, WHEEL_RADIUS_CM);

    while (1) {
        // Print all important encoder info every 500ms
        printf("Ticks: %d, RPM: %.2f, Speed: %.2f cm/s, Distance: %.2f cm, Rotations: %.2f\n",
               encoder_get_ticks(),               // Raw tick count
               encoder_calculate_rpm(),           // Computed RPM
               encoder_get_speed_cm_s(),          // Linear speed in cm/s
               encoder_get_distance_cm(),         // Total distance traveled in cm
               encoder_get_position_rotations());  // Total rotations
        sleep_ms(500);
    }
}
