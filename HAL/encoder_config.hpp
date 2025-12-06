#ifndef ENCODER_CONFIG_HPP
#define ENCODER_CONFIG_HPP

// GPIO pin assignments for the quadrature encoder
#define ENCODER_PIN_A 10   // Encoder channel A connected to GPIO 10
#define ENCODER_PIN_B 11   // Encoder channel B connected to GPIO 11

// Encoder counts per revolution (CPR)
// This is the number of pulses the encoder generates per full rotation
#define ENCODER_CPR 330

// Radius of the wheel in centimeters, used for linear speed and distance calculation
#define WHEEL_RADIUS_CM 3.0f

#endif // ENCODER_CONFIG_HPP
