#include "StaticMeshActor.h"
#include <libultra/gbi.h>
#include "Matrix.h"

extern "C" {
#include "main.h"
#include "math_util.h"
#include "math_util_2.h"
}

StaticMeshActor::StaticMeshActor(std::string name, FVector pos, IRotator rot, FVector scale, std::string model, int32_t* collision) : Name(name), Pos(pos), Rot(rot), Scale(scale), Model(""), CollisionMesh(collision) {
    Name = "StaticMesh";
    ResourceName = "hm:static_mesh";

}

void StaticMeshActor::Draw() {
    Mat4 mtx;
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    if (!Model.empty()) {
        ApplyMatrixTransformations(mtx, Pos, Rot, Scale);
        if (render_set_position(mtx, 0) != 0) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)Model.c_str());
        }
    }
}

void StaticMeshActor::Destroy() {
    bPendingDestroy = true;
}
