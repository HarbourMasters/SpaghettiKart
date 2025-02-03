#include "MoleGroup.h"
#include "Mole.h"

extern "C" {
#include "code_80057C60.h"
#include "update_objects.h"
#include "math_util.h"
#include "math_util_2.h"
}

OMoleGroup::OMoleGroup(std::vector<FVector> spawns) {
    for (auto& pos : spawns) {
        pos.x * xOrientation;
        OMole* ptr = reinterpret_cast<OMole*>(gWorldInstance.AddObject(new OMole(pos, this)));
        _moles.push_back({ptr, pos, false});
    }
}

void OMoleGroup::Tick() {
    for (auto &mole : _moles) {
        if (gObjectList[mole.Mole->_objectIndex].state == 0) {
            func_80081FF4(mole.Mole->_objectIndex);
        }
    }
}

void OMoleGroup::func_80081FF4(s32 objectIndex) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 groupSize;
    s16 mole;
    s16 offset;
    s8* activations;

    init_object(objectIndex, 0);
    gObjectList[objectIndex].unk_04C = random_int(30) + 5;

    // switch (arg1) {
    //     case 1:
    //         activations = D_8018D198;
    //         groupSize = NUM_GROUP1_MOLES;
    //         offset = 0;
    //         break;
    //     case 2:
    //         activations = D_8018D1A8;
    //         groupSize = NUM_GROUP2_MOLES;
    //         offset = 24;
    //         // offset = NUM_GROUP1_MOLES;
    //         break;
    //     case 3:
    //         activations = D_8018D1B8;
    //         groupSize = NUM_GROUP3_MOLES;
    //         offset = 57;
    //         // offset = NUM_GROUP1_MOLES + NUM_GROUP2_MOLES;
    //         break;
    // }
    // mole = random_int(groupSize);
    // for (size_t i = 0; i < groupSize; i++) {
    //     if (activations[mole] == ACTIVE) {
    //         mole++;
    //         if (mole == groupSize) {
    //             mole = 0;
    //         }
    //     } else { // not active
    //         activations[mole] = ACTIVE;
    //         gObjectList[objectIndex].type = mole;
    //        // break;
    //     }
    // }

// reimpl
    mole = random_int(_moles.size());
    for (size_t i = 0; i < _moles.size(); i++) {
        if (_moles[mole].Active == true) {
            mole++;
            if (mole == _moles.size()) {
                mole = 0;
            }
        } else { // if not active
            _moles[mole].Active = true;
            gObjectList[objectIndex].type = mole;
           // break;
        }
    }




    /*
    Ideally `gMoleSpawns` wouldn't be a union at all and its just be a list of Vec3s
    Even more ideally each mole group would have its own array for its spawns
    gObjectList[objectIndex].origin_pos[0] = gMoleSpawns.asVec3sList[offset + var_v1][0] * xOrientation;
    gObjectList[objectIndex].origin_pos[1] = gMoleSpawns.asVec3sList[offset + var_v1][1] - 9.0;
    gObjectList[objectIndex].origin_pos[2] = gMoleSpawns.asVec3sList[offset + var_v1][2];
    */
    // gObjectList[objectIndex].origin_pos[0] = gMoleSpawns.asFlatList[offset + (mole * 3) + 0] * xOrientation;
    // gObjectList[objectIndex].origin_pos[1] = gMoleSpawns.asFlatList[offset + (mole * 3) + 1] - 9.0;
    // gObjectList[objectIndex].origin_pos[2] = gMoleSpawns.asFlatList[offset + (mole * 3) + 2];

    gObjectList[objectIndex].origin_pos[0] = _moles[mole].Pos.x * xOrientation;
    gObjectList[objectIndex].origin_pos[1] = _moles[mole].Pos.y - 9.0;
    gObjectList[objectIndex].origin_pos[2] = _moles[mole].Pos.z;

}
