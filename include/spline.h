#ifndef SPLINE_H
#define SPLINE_H

#include <common_structs.h>

/*
These are some very good videos about splines/Bezier curves in general

The Beauty of Bezier Curves
https://www.youtube.com/watch?v=aVwxzDHniEw

The Continuity of Splines
https://www.youtube.com/watch?v=jvPPXbo87ds
*/

/*
This stuff is all about the b-splines used objects like Lakitu and the Boos on Banshee Boardwalk
For splines used during the award ceremony and credits, see src/ending/ceremony_and_credits.h
*/

typedef struct {
    /* 0x0 */ Vec3s pos;
    // Don't really know what to call this member.
    // It somehow controls the speed of travel along a segment of the spline but I don't really get how it works
    /* 0x6 */ s16 velocity;
} SplineControlPoint; // size = 0x8

// WARNING!!!
// You really, really shouldn't use this type for actual spline data. This is intended as a generic SplineData
// type. I would use a union, but that would cause significant ugliness elsewhere in the codebase
typedef struct {
    // This name is a little misleading
    // The control point arrays have more control points in them than this number indicates. Not sure why though.
    s16 numControlPoints;
    // This has to be an array for this to work, so just make it size 1
    SplineControlPoint controlPoints[];
} SplineData;

// All other SplineDataXX types are for use as data only. The size of the array in them matters a lot.
// But object structs should never have members with these types, just use the plain SplineData type

// Ghosts in BansheeBoardwalk x 2
// Seagulls in KoopaTroopaBeach x 2
// Penguins in Sherbet Land x 1
// length of 23

// Ghosts in BansheeBoardwalk x 2
// Seagulls in KoopaTroopaBeach x 1
// length of 24

// Ghosts in BansheeBoardwalk x 1
// Seagulls in KoopaTroopaBeach x 1
// Penguins in Sherbet Land x 1
// length of 25

// Data of this type is unreferenced or only referenced in an unused function
// lenght of 4

// Lakitu Countdown
// length of 15

// Lakitu Checkered Flag
// length of 21

// length of 13

// Lakitu Second/Final Lap
// length of 12

// Lakitu Reverse
// length of 8

#endif
