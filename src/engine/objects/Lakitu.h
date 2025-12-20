#pragma once

#include <libultraship.h>
#include <vector>
#include "Object.h"
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
 * Note that you only want 1 lakitu spawned in per human player
 * Otherwise Lakitu will animate faster than normal.
 */
class OLakitu : public OObject {
public:
    enum LakituType : uint32_t {
        STARTER = 1,
        FINISH, // Checkered flag
        TOW, // Picks up an out of bounds player
        SECOND_LAP,
        FINAL_LAP,
        REVERSE,
        TOW_ICE_CUBE, // Picks up an out of bounds player in sherbet land as an ice-cube
    };

    enum LakituTowType : uint16_t {
        NORMAL,
        ICE, // Used in sherbet land to put an ice-cube on the player
    };

public:
    explicit OLakitu(s32 playerId, LakituType type);
    ~OLakitu() {
        _count -= 1;
    }

    void Activate(LakituType type); // Triggers Lakitu into a behaviour

    virtual void Tick() override;
    virtual void Tick60fps() override;
    virtual void Draw(s32 playerId) override;

    void func_80078F64();
    void func_80079054(s32 playerId);
    void func_80079084(s32 playerId);
    void func_800790B4(s32 playerId);
    void func_800790E4(s32 playerId);
    void func_80079114(s32 objectIndex, s32 playerId, s32 arg2);
    void func_800791F0(s32 objectIndex, s32 playerId);
    void init_obj_lakitu_starter_and_checkered_flag(s32 objectIndex, s32 playerId);
    void update_object_lakitu_starter(s32 objectIndex, s32 arg1);
    void func_800729EC(s32 objectIndex);
    void init_obj_lakitu_checkered_flag(s32 objectIndex, s32 playerIndex);
    void update_object_lakitu_checkered_flag(s32 objectIndex, s32 playerIndex);
    void func_800797AC(s32 playerId);
    void func_80079860(s32 playerId);
    void func_8007993C(s32 objectIndex, Player* player);
    void init_obj_lakitu_red_flag_fishing(s32 objectIndex, s32 arg1);
    void func_80079A5C(s32 objectIndex, UNUSED Player* player);
    void update_object_lakitu_fishing(s32 objectIndex, s32 playerId);
    void update_object_lakitu_fishing2(s32 objectIndex, s32 playerId);
    void func_8007A060(s32 objectIndex, s32 playerIndex);
    void update_object_lakitu_second_lap(s32 objectIndex, s32 playerIndex);
    void func_8007A228(s32 objectIndex, s32 playerIndex);
    void update_object_lakitu_final_lap(s32 objectIndex, s32 playerIndex);
    void func_8007A3F0(s32 objectIndex, s32 arg1);
    void update_object_lakitu_reverse(s32 objectIndex, s32 playerId);
    void func_8007A66C(s32 objectIndex);
    void func_8007A778(s32 objectIndex);
    void func_8007A88C(s32 playerId);
    void func_8007A910(s32 arg0);
    void func_8007AA44(s32 playerId); // animate lakitu

private:
    static size_t _count;
    size_t _idx;
    LakituType _type;
    s32 mPlayerId;
    s32 mCameraId;
    s32 mLakituId;
};
