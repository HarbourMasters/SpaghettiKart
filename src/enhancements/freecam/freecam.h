#ifndef FREECAM_H
#define FREECAM_H

#include <libultraship.h>

void freecam(Camera*, Player*, s8);
extern f32 gFreecamSpeed;
extern f32 gFreecamSpeedMultiplier;
extern f32 gFreecamRotateSmoothingFactor;

#endif // FREECAM_H
