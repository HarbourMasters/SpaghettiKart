#ifndef FREECAM_H
#define FREECAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libultraship.h>
#include "camera.h"

void freecam(Camera*, Player*, s8);
void on_freecam(void);
void off_freecam(void);
void freecam_loop(Camera*);
void freecam_update_controller(void);
void freecam_mouse_manager(Camera*, Vec3f);
void freecam_keyboard_manager(Camera*, Vec3f);

extern f32 gFreecamSpeed;
extern f32 gFreecamSpeedMultiplier;
extern f32 gFreecamRotateFollowFactor;
extern u32 fRankIndex;
extern u32 bFreecamUseController;

#ifdef __cplusplus
}
#endif

#endif // FREECAM_H
