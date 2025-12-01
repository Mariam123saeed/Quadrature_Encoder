#include "encoder_service.h"
#include "hardware/timer.h"
#include <stdio.h>

// ---- Static variables to track encoder state ----
static int32_t last_ticks = 0;          // Tick count at previous sampling
static int32_t current_ticks = 0;       // Current tick count
static uint _cpr = 0;                   // Counts per revolution
static float _wheel_radius = 0.0f;      // Wheel radius in cm
static float _rpm = 0.0f;               // Calculated RPM
static float _speed_cm_s = 0.0f;        // Linear speed in cm/s
static float _distance_cm = 0.0f;       // Total distance traveled in cm


// ---- Timer callback for periodic calculation ----
// This function runs every 100ms to update RPM, speed, and distance
static bool repeating_timer_callback(struct repeating_timer *t) {
    current_ticks = encoder_get_ticks();         // Read current encoder ticks
    int32_t delta = current_ticks - last_ticks;  // Calculate ticks since last update

    // Calculate RPM: ticks * 60 / 0.1s / CPR = ticks * 600 / CPR
    _rpm = ((float)delta * 600.0f) / (float)_cpr;  
    // Calculate total distance in cm: (delta / CPR) * 2 * pi * wheel_radius
    _distance_cm += ((float)delta / (float)_cpr) * (2.0f * 3.1415926f * _wheel_radius);
    // Calculate linear speed in cm/s over the 100ms interval
    _speed_cm_s = _distance_cm / 0.1f;  

    last_ticks = current_ticks;       // Update last tick count for next iteration
    return true;                      // Keep repeating the timer
}

// ---- Initialize service layer ----
void encoder_service_init(uint pin_a, uint pin_b, uint cpr, float wheel_radius_cm) {
    encoder_init(pin_a, pin_b);
    _cpr = cpr;
    _wheel_radius = wheel_radius_cm;
    
    // Start repeating timer to update encoder metrics every 100ms
    static struct repeating_timer timer;
    add_repeating_timer_ms(100, repeating_timer_callback, NULL, &timer);
}

// ---- Public API functions ----
float encoder_calculate_rpm() {
    return _rpm;     // Return the last calculated RPM
}

float encoder_get_speed_cm_s() {
    return _speed_cm_s;  // Return linear speed in cm/s
}

float encoder_get_distance_cm() {
    return _distance_cm;   // Return total distance traveled
}

float encoder_get_position_rotations() {
    return (float)current_ticks / (float)_cpr;   // Return total wheel rotations
}
