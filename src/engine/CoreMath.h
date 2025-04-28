#ifndef CORE_MATH_H
#define CORE_MATH_H

#include <libultraship.h>

/**
 * @file CoreMath.h
 * 
 * Basic vector structs for manipulating 2D and 3D coordinates
 * 
 */

/**
 * 
 * Applies pos, rot, and scale
 *
 */
struct FVector {
    float x, y, z;

#ifdef __cplusplus
    // Operator to add two FVector objects
    FVector operator+(const FVector& other) const {
        return FVector(x + other.x, y + other.y, z + other.z);
    }

    // Operator to subtract two FVector objects
    FVector operator-(const FVector& other) const {
        return FVector(x - other.x, y - other.y, z - other.z);
    }

    // Operator to multiply a FVector by a scalar (float)
    FVector operator*(float scalar) const {
        return FVector(x * scalar, y * scalar, z * scalar);
    }

    float Dot(const FVector& other) const {
        return x * other.x + y * other.y + z * other.z;
    }


    FVector Cross(const FVector& other) const {
        return FVector(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float Magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    FVector Normalize() const {
        float len = std::sqrt(x * x + y * y + z * z);
        if (len > 0.0001f) {
            return FVector(
                x / len, y / len, z / len
            );
        }
        return FVector(0, 0, 0);
    }

    FVector() : x(0), y(0), z(0) {}
    FVector(float x, float y, float z) : x(x), y(y), z(z) {}
#endif // __cplusplus
};

struct FVector4 {
    float x, y, z, w;

#ifdef __cplusplus

    FVector4() : x(0), y(0), z(0), w(0) {}
    FVector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
#endif // __cplusplus
};

/**
 * For providing X and Z when you do not need Y
 * Some actors set themselves on the surface automatically
 * which means it does not use a Y coordinate
 * The train follows a set Y value. The hedgehog's patrolPoint only uses X and Z.
 */
struct FVector2D {
    float x, z;

#ifdef __cplusplus
    FVector2D& operator=(const FVector2D& other) {
        x = other.x;
        z = other.z;
        return *this;
    }

    FVector2D() : x(0), z(0) {}
    FVector2D(float x, float z) : x(x), z(z) {}
#endif // __cplusplus
};

// Sets integer X Z coordinates
typedef struct IVector2D {
    int32_t X, Y;

#ifdef __cplusplus
    IVector2D() : X(0), Y(0) {}  // Default constructor

    IVector2D(int32_t x, int32_t y) : X(x), Y(y) {}  // Constructor to initialize with values


    IVector2D& operator=(const IVector2D& other) {
        X = other.X;
        Y = other.Y;
        return *this;
    }
#endif // __cplusplus
} IVector2D;

/**
 * This struct immediately converts float pitch/yaw/roll in degrees to n64 int16_t binary angles 0-0xFFFF == 0-360 degrees
 * ToDegrees() Receive an FRotator of float degrees back.
 * Set() Set an n64 int16_t binary angles 0-0xFFFF
 */
struct IRotator {
    uint16_t pitch, yaw, roll;

#ifdef __cplusplus
    IRotator& operator=(const IRotator& other) {
        pitch = other.pitch;
        yaw   = other.yaw;
        roll  = other.roll;
        return *this;
    }

    [[nodiscard]] void Set(uint16_t p, uint16_t y, uint16_t r) {
        pitch = p;
        yaw = y;
        roll = r;
    }

    IRotator() : pitch(0), yaw(0), roll(0) {}
    IRotator(float p, float y, float r) {
        pitch = p * (UINT16_MAX / 360);
        yaw   = y * (UINT16_MAX / 360);
        roll  = r * (UINT16_MAX / 360);
    }

    // Convert to radians as FVector
    [[nodiscard]] FVector ToRadians() const {
        float scale = 2.0f * M_PI / 65536.0f;
        return FVector(
            pitch * scale,
            yaw   * scale,
            roll  * scale
        );
    }
#endif // __cplusplus
};

/**
 * Use IRotator unless you want to do some math in degrees.
 * Always use ToBinary() or Rotator when sending into matrices or apply translation functions
 * Convert from IRotator to FRotator float degrees by doing FRotator(myIRotator);
 */
struct FRotator {
    float pitch, yaw, roll;

#ifdef __cplusplus
    FRotator& operator=(const FRotator& other) {
        pitch = other.pitch;
        yaw   = other.yaw;
        roll  = other.roll;
        return *this;
    }

    // Convert to binary rotator 0 --> INT16_MAX
    [[nodiscard]] IRotator ToBinary() const {
        return IRotator(
            static_cast<uint16_t>(pitch * (UINT16_MAX / 360)),
            static_cast<uint16_t>(yaw   * (UINT16_MAX / 360)),
            static_cast<uint16_t>(roll  * (UINT16_MAX / 360))
        );
    }

    FRotator() : pitch(0), yaw(0), roll(0) {}
    FRotator(float p, float y, float r) : pitch(p), yaw(y), roll(r) {}
    FRotator(IRotator rot) {
        pitch = static_cast<float>(rot.pitch * (360 / UINT16_MAX));
        yaw   = static_cast<float>(rot.yaw   * (360 / UINT16_MAX));
        roll  = static_cast<float>(rot.roll  * (360 / UINT16_MAX));
    }
#endif // __cplusplus
};

/**
 * For selecting a section of a course path
 * Usage: IPathSpan(point1, point2) --> IPathSpan(40, 65)
 */
struct IPathSpan {
    int Start, End;

#ifdef __cplusplus
    // Default Constructor
    IPathSpan() : Start(0), End(0) {}

    // Parameterized Constructor
    IPathSpan(int InStart, int InEnd)
        : Start(InStart), End(InEnd) {}

    // Copy Assignment Operator
    IPathSpan& operator=(const IPathSpan& Other) {
        if (this != &Other) { // Avoid self-assignment
            Start = Other.Start;
            End = Other.End;
        }
        return *this;
    }

    // Equality Operator
    bool operator==(const IPathSpan& Other) const {
        return Start == Other.Start && End == Other.End;
    }

    // Inequality Operator
    bool operator!=(const IPathSpan& Other) const {
        return !(*this == Other);
    }
#endif // __cplusplus
};

#endif // CORE_MATH_H
