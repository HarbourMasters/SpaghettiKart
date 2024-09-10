#ifndef FREECAM_H
#define FREECAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libultraship.h>
#include "camera.h"

void freecam(Camera*, Player*, s8);
void freecam_render_setup(void);

extern f32 gFreecamSpeed;
extern f32 gFreecamSpeedMultiplier;
extern f32 gFreecamRotateSmoothingFactor;
extern u32 fRankIndex;
extern u32 gFreecamControllerType;

#ifdef __cplusplus
}
#endif

#endif // FREECAM_H
