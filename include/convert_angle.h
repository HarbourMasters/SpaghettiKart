#ifndef CONVERT_ANGLE_H
#define CONVERT_ANGLE_H

#include <math.h> // For M_PI and other mathematical functions if necessary

// Ensure s16 is defined. This often comes from <libultraship.h> or a common types file.
// If not, you might need something like:
// #include <stdint.h> // For int16_t
// typedef int16_t s16;

// Basic constants for conversions
#define N64_ANGLE_UNITS_PER_CIRCLE 65536.0f
#define DEGREES_PER_CIRCLE 360.0f
#define RADIANS_PER_CIRCLE (2.0f * M_PI)

// --- Conversion from N64 angle units (0-65535 for a full circle) ---

/**
 * Converts an N64 angle value to radians.
 * N64 angles are often s16 or u16 where 65535 equals 2*PI radians.
 */
#define N64_ANGLE_TO_RADIANS(n64_angle) \
    (((float)(n64_angle) / N64_ANGLE_UNITS_PER_CIRCLE) * RADIANS_PER_CIRCLE)

/**
 * Converts an N64 angle value to degrees.
 */
#define N64_ANGLE_TO_DEGREES(n64_angle) \
    (((float)(n64_angle) / N64_ANGLE_UNITS_PER_CIRCLE) * DEGREES_PER_CIRCLE)

// --- Conversion from Radians ---

/**
 * Converts an angle value in radians to N64 angle units (typically s16).
 */
#define RADIANS_TO_N64_ANGLE(rad_angle) \
    ((s16)(((rad_angle) / RADIANS_PER_CIRCLE) * N64_ANGLE_UNITS_PER_CIRCLE))

/**
 * Converts an angle value in radians to degrees.
 */
#define RADIANS_TO_DEGREES(rad_angle) \
    ((rad_angle) * (DEGREES_PER_CIRCLE / RADIANS_PER_CIRCLE)) // Equivalent to (rad_angle) * (180.0f / M_PI)

// --- Conversion from Degrees ---

/**
 * Converts an angle value in degrees to N64 angle units (typically s16).
 */
#define DEGREES_TO_N64_ANGLE(deg_angle) \
    ((s16)(((deg_angle) / DEGREES_PER_CIRCLE) * N64_ANGLE_UNITS_PER_CIRCLE))

/**
 * Converts an angle value in degrees to radians.
 */
#define DEGREES_TO_RADIANS(deg_angle) \
    ((deg_angle) * (RADIANS_PER_CIRCLE / DEGREES_PER_CIRCLE)) // Equivalent to (deg_angle) * (M_PI / 180.0f)

#endif // CONVERT_ANGLE_H