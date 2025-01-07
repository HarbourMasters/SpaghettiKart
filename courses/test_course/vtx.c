#include "mk64.h"
#include "common_structs.h"

CourseVtx test_course_vertices[] = {
    // Top-left vertex (x, y, z), texture coord (tc), color and alpha (ca)
    { { -100, 100, 0 }, { 0, 0 }, { MACRO_COLOR_FLAG(0xFF, 0x00, 0x00, 0), 0x00 } }, // Red

    // Top-right vertex
    { { 100, 100, 0 }, { 1024, 0 }, { MACRO_COLOR_FLAG(0x00, 0xFF, 0x00, 0), 0x00 } }, // Green

    // Bottom-right vertex
    { { 100, -100, 0 }, { 1024, 1024 }, { MACRO_COLOR_FLAG(0x00, 0x00, 0xFF, 0), 0x00 } }, // Blue

    // Bottom-left vertex
    { { -100, -100, 0 }, { 0, 1024 }, { MACRO_COLOR_FLAG(0xFF, 0xFF, 0x00, 0), 0x00 } }, // Yellow
};
