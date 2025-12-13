#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"

#include "RegisterContent.h"
#include "World.h"
#include "CoreMath.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "vehicles.h"
#include "waypoints.h"
#include "common_structs.h"
#include "objects.h"
#include "camera.h"
#include "some_data.h"
}

/**
 * OBoos
 * 
 * Args use path indices or path points. Recommended to use a span of 10
 * for each IPathSpan as that's how the original game did it. Ex. IPathSpan(10, 20)
 * 
 * Once boos are active, they do not deactivate until the player drives into leftBoundary
 * or rightBoundary. leftBoundary is really only used to deactivate the boos when the player is
 * driving in the reverse direction.
 * 
 * @arg numBoos to show, default 5, maximum 10 allowed due to limited splines
 * @arg leftBoundary When the player enters this area, deactivate the boos.
 * @arg active When the player enters this area, activate the boos.
 * @arg rightBoundary When the player enters this area, deactivate the boos.
 * 
 */
class OBoos : public OObject {
public:
    // This is simply a helper function to keep Spawning code clean
    static inline OBoos* Spawn(size_t numBoos, const IPathSpan& leftBoundary, const IPathSpan& triggerBoundary, const IPathSpan& rightBoundary) {
        SpawnParams params = {
            .Name = "mk:boos",
            .Count = numBoos,
            .LeftExitSpan = leftBoundary,
            .TriggerSpan = triggerBoundary,
            .RightExitSpan = rightBoundary,
        };
        return static_cast<OBoos*>(AddObjectToWorld<OBoos>(params));
    }

    explicit OBoos(const SpawnParams& params);

    ~OBoos() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    virtual void DrawEditorProperties() override;
    void func_800523B8(s32 objectIndex, s32 arg1, u32 arg2);

    void func_8007CA70(void);
    void func_8007C5B4(s32 objectIndex);
    void func_8007C684(s32 objectIndex);
    void func_8007C4A4(s32 objectIndex);
    s32 func_8007C9F8(void);
    void BooStart(s32 someIndex, s32 arg1);
    void BooExit(s32 someIndex);
    void func_8007C550(s32 objectIndex);

    IPathSpan LeftTrigger;
    IPathSpan ActiveZone;
    IPathSpan RightTrigger;
private:
    FVector _pos;
    static size_t _count;
    size_t _idx;
    std::vector<s32> _indices; // indices into gObjectList
    size_t _numBoos;

    bool _isActive = false;
    s32 _playerId = 0;
};
