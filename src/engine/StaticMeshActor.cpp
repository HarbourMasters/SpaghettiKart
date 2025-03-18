#include "StaticMeshActor.h"
#include <libultra/gbi.h>
#include "Matrix.h"

extern "C" {
#include "main.h"
#include "math_util.h"
#include "math_util_2.h"
}

StaticMeshActor::StaticMeshActor(std::string name, FVector pos, IRotator rot, FVector scale, std::string model, int32_t* collision) : Name(name), Pos(pos), Rot(rot), Scale(scale), Model(""), Collision(collision) {

}

void StaticMeshActor::Draw() {
    Mat4 mtx;
printf("DRAWING \n");
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    if (!Model.empty()) {
        ApplyMatrixTransformations(mtx, Pos, Rot, Scale);
        if (render_set_position(mtx, 0) != 0) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)Model.c_str());
        }
    }

    FVector test = FVector(cameras[0].pos[0], cameras[0].pos[1], cameras[0].pos[2]);
    FVector diff = test - Pos;
    printf("Cam dist: %f", diff.Magnitude());
}

void StaticMeshActor::Destroy() {
    bPendingDestroy = true;
}
