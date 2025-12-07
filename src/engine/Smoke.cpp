#include <libultraship.h>
#include <libultra/gbi.h>

#include "Smoke.h"
#include "World.h"
#include "vehicles/Train.h"
#include "vehicles/Boat.h"
#include <port/interpolation/FrameInterpolation.h>

extern "C" {
#include "macros.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "update_objects.h"
#include "math_util_2.h"
#include "assets/models/common_data.h"
#include "assets/textures/common_data.h"
}

//! @todo: This should be an Object class one day
void TrainSmokeTick() {
    s32 count;
    s32 i;
    s32 temp_a0;
    Object* object;

    for (auto& actor : gWorldInstance.Actors) {
        if (auto train = dynamic_cast<ATrain*>(actor)) {
            if (train->SmokeTimer != 0) {
                train->SmokeTimer -= 1;
            }

            if ((train->SomeFlags != 0) || (train->SmokeTimer != 0)) {
                count = 0;
                for (i = 0; i < 128; i++) {
                    temp_a0 = train->SmokeParticles[i];
                    if (temp_a0 != -1) {
                        object = &gObjectList[temp_a0];
                        if (object->state != 0) {
                            func_80075714(temp_a0);
                            if (object->state == 0) {
                                delete_object_wrapper(&train->SmokeParticles[i]);
                            }
                            count += 1;
                        }
                    }
                }
                if (count != 0) {
                    train->SmokeTimer = 100;
                }
            }
        } else if (auto boat = dynamic_cast<ABoat*>(actor)) {
            if (boat->SmokeTimer != 0) {
                boat->SmokeTimer -= 1;
            }
            if ((boat->SomeFlags != 0) || (boat->SmokeTimer != 0)) {
                count = 0;
                for (size_t i = 0; i < 128; i++) {
                    temp_a0 = boat->SmokeParticles[i];
                    if (temp_a0 != -1) {
                        object = &gObjectList[temp_a0];
                        if (object->state != 0) {
                            func_80075B84(temp_a0);
                            if (object->state == 0) {
                                delete_object_wrapper(&boat->SmokeParticles[i]);
                            }
                            count += 1;
                        }
                    }
                }
                if (count != 0) {
                    boat->SmokeTimer = 100;
                }
            }
        }
    }
}

void TrainSmokeDraw(s32 cameraId) {
    Camera* camera = &camera1[cameraId];

    for (auto& actor : gWorldInstance.Actors) {
        if (auto train = dynamic_cast<ATrain*>(actor)) {
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007AE0);
            load_texture_block_i8_nomirror((uint8_t*) D_0D029458, 32, 32);
            func_8004B72C(255, 255, 255, 255, 255, 255, 255);
            D_80183E80[0] = 0;
            D_80183E80[2] = 0x8000;

            if ((train->SomeFlags != 0) && (is_particle_on_screen(train->Locomotive.position, camera, 0x4000U) != 0)) {
                for (size_t i = 0; i < 128; i++) {
                    FrameInterpolation_RecordOpenChild("TrainSmokeParticle", train->SmokeParticles[i]);
                    render_object_train_smoke_particle(train->SmokeParticles[i], cameraId);
                    FrameInterpolation_RecordCloseChild();
                }
            }
        } else if (auto boat = dynamic_cast<ABoat*>(actor)) {
            gSPDisplayList(gDisplayListHead++, (Gfx*) D_0D007AE0);

            load_texture_block_i8_nomirror((uint8_t*) D_0D029458, 32, 32);
            func_8004B72C(255, 255, 255, 255, 255, 255, 255);
            D_80183E80[0] = 0;
            D_80183E80[2] = 0x8000;
            if ((boat->SomeFlags != 0) && (is_particle_on_screen(boat->Position, camera, 0x4000U) != 0)) {
                for (size_t i = 0; i < gObjectParticle2_SIZE; i++) {
                    FrameInterpolation_RecordOpenChild("BoatSmokeParticle", boat->SmokeParticles[i]);
                    render_object_paddle_boat_smoke_particle(boat->SmokeParticles[i], cameraId);
                    FrameInterpolation_RecordCloseChild();
                }
            }
        }
    }
}
