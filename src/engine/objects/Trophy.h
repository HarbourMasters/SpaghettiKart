#pragma once

#include <libultraship.h>
#include "Object.h"
#include "RegisterContent.h"
#include "World.h"
#include "engine/particles/StarEmitter.h"

extern "C" {
#include "common_structs.h"
}

class OTrophy : public OObject {
public:
    enum TrophyType : int16_t {
        BRONZE,
        SILVER,
        GOLD,
        BRONZE_150,
        SILVER_150,
        GOLD_150,
    };

    enum Behaviour : int16_t {
        PODIUM_CEREMONY,
        STATIONARY,
        ROTATE, // A dual-axis opposing rotation
        ROTATE2, // A single-axis rotation
        GO_FISH,
    };

    // This is simply a helper function to keep Spawning code clean
    static OTrophy* Spawn(const FVector& pos, TrophyType trophy, Behaviour bhv) {
        SpawnParams params = {
            .Name = "mk:trophy",
            .Type = trophy,
            .Behaviour = bhv,
            .Location = pos,
        };
        return dynamic_cast<OTrophy*>(AddObjectToWorld<OTrophy>(params));
    }

    explicit OTrophy(const SpawnParams& params);

    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void DrawEditorProperties() override;
    void func_80086700(s32 objectIndex);
    void func_80086940(s32 objectIndex);
    void func_80086C14(s32 objectIndex);
    void func_80086C6C(s32 objectIndex);

private:
    StarEmitter* _emitter;

    TrophyType _type;
    Behaviour _bhv;
    int8_t _toggle;
    int8_t *_toggleVisibility;
    Vec3f _oldPos;
    bool _isMod = false;
};
