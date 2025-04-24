#pragma once

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "Collision.h"
#include "GameObject.h"

namespace Editor {

class Gizmo {
public:

    enum class GizmoHandle {
        None,
        All_Axis,
        X_Axis,
        Y_Axis,
        Z_Axis
    };

    enum class TranslationMode {
        Move,
        Rotate,
        Scale
    };

    void Tick();
    void Draw();
    void Load();

    void SetGizmo(GameObject* object, Ray ray);
    void SetGizmoNoCursor(GameObject* object); // Used for scene explorer selection
    void Translate();
    void Rotate();
    void Scale();
    void DrawHandles();
    f32 SnapToSurface(FVector* pos);

    struct TrackDimensions {
        s16 MinX = -10000;
        s16 MaxX = 10000;
        s16 MinY = -3000;
        s16 MaxY = 3000;
        s16 MinZ = -10000;
        s16 MaxZ = 10000;
    };
    TrackDimensions dimensions;

    bool Enabled;
    bool ManipulationStart = true;
    FVector InitialScale = {1, 1, 1};
    IRotator InitialRotation = {0, 0, 0};
    GizmoHandle SelectedHandle;

    GameObject RedCollision;
    GameObject GreenCollision;
    GameObject BlueCollision;

    GameObject RedRotateCollision;
    GameObject GreenRotateCollision;
    GameObject BlueRotateCollision;

    GameObject RedScaleCollision;
    GameObject GreenScaleCollision;
    GameObject BlueScaleCollision;

    MtxF Mtx_RedX;
    MtxF Mtx_GreenY;
    MtxF Mtx_BlueZ;

    FVector Pos; // Global scene view
    IRotator Rot = {0, 0, 0};
    float AllAxisRadius = 3.0f; // Free move selection radius
    float PickDistance;
    FVector _cursorOffset;
    float _gizmoOffset = 8.0f;

    float HandleSize = 2.0f;
    
    FVector _ray;
    GameObject* _selected = nullptr;
    private:
    bool _draw = false;
};
}
