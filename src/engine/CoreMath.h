#pragma once

#include <libultraship.h>

/**
 * @file CoreMath.h
 *
 * Basic vector structs for manipulating 2D and 3D coordinates
 *
 */

struct FVector {
    float x, y, z;

    FVector& operator=(const FVector& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

/**
 * For providing X and Z when you do not need Y
 * Some actors set themselves on the surface automatically
 * which means it does not use a Y coordinate
 * The train follows a set Y value. The hedgehog's patrolPoint only uses X and Z.
 */
struct FVector2D {
    float x, z;

    FVector2D& operator=(const FVector2D& other) {
        x = other.x;
        z = other.z;
        return *this;
    }
};

// Sets integer X Z coordinates
struct IVector2D {
    int32_t X, Z;

    IVector2D() : X(0), Z(0) {
    } // Default constructor

    IVector2D(int32_t x, int32_t z) : X(x), Z(z) {
    } // Constructor to initialize with values

    IVector2D& operator=(const IVector2D& other) {
        X = other.X;
        Z = other.Z;
        return *this;
    }
};

struct FRotation {
    float pitch, yaw, roll;

    FRotation& operator=(const FRotation& other) {
        pitch = other.pitch;
        yaw = other.yaw;
        roll = other.roll;
        return *this;
    }
};

/**
 * For selecting a section of a course path
 * Usage: IPathSpan(point1, point2) --> IPathSpan(40, 65)
 */
struct IPathSpan {
    int Start, End;

    // Default Constructor
    IPathSpan() : Start(0), End(0) {
    }

    // Parameterized Constructor
    IPathSpan(int InStart, int InEnd) : Start(InStart), End(InEnd) {
    }

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
};
