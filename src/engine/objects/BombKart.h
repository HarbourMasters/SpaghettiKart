#pragma once

#include <libultraship.h>
#include "engine/objects/Object.h"
#include <vector>
#include "engine/Matrix.h"

#include "World.h"

extern "C" {
#include "macros.h"
#include "main.h"
#include "vehicles.h"
#include "waypoints.h"
#include "common_structs.h"
#include "objects.h"
}

/**
 * Used in VS mode
 * 
 * This differs from the other vehicle classes in that it does not get added to the standard actor list
 * So this is sort of its own thing. Draw call in different place too.
 */
class OBombKart : public OObject {
public:
    enum States : uint16_t { // 0,1,3,5
        DISABLED,
        CCW,
        CW,
        STATIONARY,
        CHASE,
        EXPLODE,
        PODIUM_CEREMONY,
    };

    const char* Type;

    Vec3f Pos;
    Vec3f WheelPos[4]; //! @todo Turn WheelPos into a struct
    f32 Unk_3C;
    u16 SomeRot; // Some angle
    u16 WaypointIndex; // The waypoint the kart circles
    States State = States::DISABLED;
    u16 BounceTimer = 0;
    u16 CircleTimer = 0;
    u16 Unk_4A = 0;
    s16 Unk_4C = 1;
    f32 CenterY; // Center of the circle
    s32 ObjectIndex = 0; // Index into gObjectList
    Collision _Collision;

    // Set waypoint to NULL if using a spawn position and not a waypoint.
    explicit OBombKart(Vec3f pos, TrackWaypoint* waypoint, uint16_t waypointIndex, uint16_t state, f32 unk_3C);

    ~OBombKart() {
        _count--;
    }

    static size_t GetCount() {
        return _count;
    }

    virtual void Tick() override;
    virtual void Draw(s32 cameraId) override;
    void DrawBattle(s32 cameraId);
    void SomeRender(Vec3f arg1);
    void LoadMtx();
    void Waypoint(s32 screenId);
private:
    static size_t _count;
    s32 _idx;
    Player* FindTarget();
    void Chase(Player*, Vec3f pos);

    Vec3f _spawnPos;
    Player* _target = NULL;


};
