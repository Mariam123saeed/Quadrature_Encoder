#ifndef ENCODER_SERVICE_H
#define ENCODER_SERVICE_H

#include "encoder_hal.h"


/*Initialize the encoder service layer
pin_a, pin_b : GPIO pins connected to encoder channels
cpr : counts per revolution of the encoder
wheel_radius_cm : radius of the wheel in cm (for distance calculation)*/ 

void encoder_service_init(uint pin_a, uint pin_b, uint cpr, float wheel_radius_cm);

// Calculate RPM based on the tick count over a time interval
float encoder_calculate_rpm();

// Get linear speed in cm/s using wheel radius and RPM
float encoder_get_speed_cm_s();

// Compute total distance traveled in cm
float encoder_get_distance_cm();

// Compute total rotations of the wheel/encoder
float encoder_get_position_rotations();

#endif // ENCODER_SERVICE_H
