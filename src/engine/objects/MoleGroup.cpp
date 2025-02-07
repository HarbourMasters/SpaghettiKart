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
        } else {
            mole.Mole->func_800821AC(mole.Mole->_objectIndex, 1);
        }
    }
}

void OMoleGroup::func_80081FF4(s32 objectIndex) {
    init_object(objectIndex, 0);
    gObjectList[objectIndex].unk_04C = random_int(30) + 5;

    s16 mole = random_int(_moles.size() - 1);
    for (size_t i = 0; i < _moles.size() - 1; i++) {
        if (_moles[mole].Active == true) {
            mole++;
            if (mole == _moles.size()) {
                mole = 0;
            }
        } else { // if not active
            _moles[mole].Active = true;
            gObjectList[objectIndex].type = mole;
            break;
        }
    }

    gObjectList[objectIndex].origin_pos[0] = _moles[mole].Pos.x * xOrientation;
    gObjectList[objectIndex].origin_pos[1] = _moles[mole].Pos.y - 9.0;
    gObjectList[objectIndex].origin_pos[2] = _moles[mole].Pos.z;
}
