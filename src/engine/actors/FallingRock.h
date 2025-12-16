#pragma once

#include <libultraship.h>
#include "RegisterContent.h"
#include "engine/Actor.h"
#include "CoreMath.h"
#include "engine/SpawnParams.h"
#include "engine/CoreMath.h"

// Falls from the sky bouncing off of geography until it goes through water.
// Then after a brief period of time, respawns.
class AFallingRock : public AActor {
public:

    explicit AFallingRock(SpawnParams params);
    ~AFallingRock() {
        _count -= 1;
    };

    // This is simply a helper function to keep Spawning code clean
    // @arg respawnTimer default game used 60, 120, 180 as the timer. Time until respawn after reaching the bottom?
    static AFallingRock* Spawn(FVector pos, int16_t respawnTimer) {
        SpawnParams params = {
            .Name = "mk:falling_rock",
            .Behaviour = respawnTimer,
            .Location = pos,
        };
        return dynamic_cast<AFallingRock*>(AddActorToWorld<AFallingRock>(params));
    }

    int16_t TimerLength = 80;

    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual bool IsMod() override;
    virtual void Tick() override;
    virtual void Draw(Camera*) override;
    virtual void DrawEditorProperties() override;
    void Reset();

    private:
    uint32_t RespawnTimer = 0;
    static size_t _count;
};
