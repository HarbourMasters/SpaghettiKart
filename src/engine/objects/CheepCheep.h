#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"
#include "RegisterContent.h"
#include "engine/CoreMath.h"

#include "World.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "common_structs.h"
#include "objects.h"
#include "course_offsets.h"
#include "some_data.h"
}


class OCheepCheep : public OObject {
public:
    enum class Behaviour : int16_t {
        RACE,
        PODIUM_CEREMONY
    };

    // This is simply a helper function to keep Spawning code clean
    static inline OCheepCheep* Spawn(const FVector& pos, Behaviour behaviour, IPathSpan span) {
        SpawnParams params = {
            .Name = "mk:cheep_cheep",
            .Behaviour = static_cast<int16_t>(behaviour),
            .Location = pos,
            .PathSpan = span,
        };
        return static_cast<OCheepCheep*>(AddObjectToWorld<OCheepCheep>(params));
    }

    explicit OCheepCheep(const SpawnParams& params);

    IPathSpan ActivationPoints; // Path points activation points

    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void DrawEditorProperties() override;
    void func_8007BBBC(s32 objectIndex);
    void func_8007BD04(s32 playerId);
    void init_var_cheep_cheep(s32 objectIndex);
    void func_8007BEC8(s32 objectIndex);
    void func_8007BFB0(s32 objectIndex);

private:

    s32 _idx;
    Behaviour _behaviour;
};
