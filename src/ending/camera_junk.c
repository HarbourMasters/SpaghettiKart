#include <libultraship.h>
#include <macros.h>
#include <common_structs.h>

#include "camera_junk.h"
#include "camera.h"
#include "math_util.h"
#include "ceremony_and_credits.h"
#include "main.h"

void update_camera_podium_ceremony(void) {
    Camera* camera;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;

    camera = &cameras[0];
    func_80283648(camera);

    x_dist = camera->lookAt[0] - camera->pos[0];
    y_dist = camera->lookAt[1] - camera->pos[1];
    z_dist = camera->lookAt[2] - camera->pos[2];
    camera->rot[1] = atan2s(x_dist, z_dist);
    camera->rot[0] = atan2s(sqrtf((x_dist * x_dist) + (z_dist * z_dist)), y_dist);
    camera->rot[2] = 0;
}

// GP podium ceremony camera settings?
void init_camera_podium_ceremony(void) {
    cameras[0].pos[0] = -3133.0f;
    cameras[0].pos[1] = 19.0f;
    cameras[0].pos[2] = -467.0f;
    cameras[0].lookAt[0] = -3478.0f;
    cameras[0].lookAt[1] = 21.0f;
    cameras[0].lookAt[2] = -528.0f;
    cameras[0].up[0] = 0.0f;
    cameras[0].up[1] = 1.0f;
    cameras[0].up[2] = 0.0f;
    gCameraFOV[0] = 40.0f;
    gScreenAspect = 1.33333333f;
    D_80150150 = 3.0f;
    D_8015014C = 6800.0f;
    init_cinematic_camera();
}
