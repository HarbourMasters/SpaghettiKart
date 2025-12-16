#pragma once

#include <libultraship.h>
#include "RegisterContent.h"
#include "engine/World.h"
#include "engine/SpawnParams.h"
#include "engine/CoreMath.h"

#include "engine/objects/Object.h"

extern "C" {
#include "common_structs.h"
#include "camera.h"
}

//! @todo Make shadow size bigger if thwomp is scaled up
//! @todo make adjustable properties for squishSize and boundingBoxSize

/**
 * Thwomp OObject
 * 
 * The game automatically places the actor on the ground so you do not need to provide a Y coordinate value.
 * 
 * @arg direction the rotational direction the thwomp is facing.
 * @arg behaviour the behaviour of the thwomp, uses values 1-6.
 * @arg primAlpha unknown
 * @arg boundingBoxSize optional. The size of the bounding box for the thwomp. Default value is 12
 */
class OThwomp : public OObject {
public:
    enum States : uint16_t {
        DISABLED,
        STATIONARY,
        MOVE_AND_ROTATE,
        MOVE_FAR, // Requires enough space to work
        STATIONARY_FAST,
        SLIDE, // Moves sideways left / right
        JAILED // Has no collision
    };

    // This is simply a helper function to keep Spawning code clean
    static OThwomp* Spawn(s16 x, s16 z, s16 direction, f32 scale, s16 behaviour, s16 primAlpha, u16 boundingBoxSize = 7) {
        IRotator rot;
        rot.Set(0, direction, 0);

        SpawnParams params = {
            .Name = "mk:thwomp",
            .Behaviour = behaviour,
            .Location = FVector(x, 0, z),
            .Rotation = rot,
            .Scale = FVector(0, scale, 0),
            .PrimAlpha = primAlpha,
            .BoundingBoxSize = boundingBoxSize
        };
        return dynamic_cast<OThwomp*>(AddObjectToWorld<OThwomp>(params));
    }

    explicit OThwomp(const SpawnParams& params);

    ~OThwomp() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void Tick60fps() override;
    virtual void Draw(s32 cameraId) override;
    virtual void DrawEditorProperties() override;
    void SetVisibility(s32 objectIndex);
    void func_80080B28(s32 objectIndex, s32 playerId);
    void DrawModel(s32);
    void TranslateThwompLights();
    void ThwompLights(s32 objectIndex);
    void func_80080DE4(s32 arg0);
    s32 func_8007F75C(s32 playerId);
    void func_8007F8D8();
    void SetPlayerCrushedEffect(s32 objectIndex, Player* player);
    void func_80080A4C(s32 objectIndex, s32 cameraPlayerId);
    void func_8007542C(s32 arg0);
    void func_80074FD8(s32 objectIndex);
    void AddParticles(s32 arg0);

    s32 func_8007E50C(s32 objectIndex, Player* player, Camera* camera);
    s32 func_8007E59C(s32 objectIndex);

    void func_8007F544(s32 objectIndex);
    void func_8007EFBC(s32 objectIndex);
    void func_8007F280(s32 objectIndex);

    void func_8007F660(s32 objectIndex, s32 arg1, s32 arg2);
    void func_80080E8C(s32 objectIndex1, s32 objectIndex2, s32 arg2);
    void func_8007F6C4(s32 objectIndex, s32 playerId);

    void func_800810F4(s32 objectIndex);
    void func_80081080(s32 objectIndex);

    void StationaryBehaviour(s32 objectIndex);
    void func_8007EC30(s32 objectIndex);

    void MoveAndRotateBehaviour(s32 objectIndex);
    void func_8007EE5C(s32 objectIndex);

    void MoveFarBehaviour(s32 objectIndex);
    void func_8007FA08(s32 objectIndex);
    void func_8007FF5C(s32 objectIndex);
    void func_8007FB48(s32 objectIndex);
    void func_8007FEA4(s32 objectIndex);

    void StationaryFastBehaviour(s32 objectIndex);
    void func_80080078(s32 objectIndex);

    void JailedBehaviour(s32 objectIndex);
    void func_800802C0(s32 objectIndex);

    void SlidingBehaviour(s32 objectIndex);
    void func_80080524(s32 objectIndex);
    void func_8008085C(s32 objectIndex);
    void func_800806BC(s32 objectIndex);
    void func_8008078C(s32 objectIndex);

    void func_8007E63C(s32 objectIndex);

    u16 BoundingBoxSize;
    OThwomp::States Behaviour;
    int16_t PrimAlpha;
private:
    static size_t _count;
    static size_t _rand;
    s32 _idx;
    s16 _faceDirection;
    //! @todo Write this better. This effects the squish size and the bounding box size.
    // We should probably return to the programmer the pointer to the actor so they can do thwomp->squishSize = value.
};
