#include "MoleGroup.h"
#include "Mole.h"

extern "C" {
#include "code_80057C60.h"
#include "update_objects.h"
#include "math_util.h"
#include "math_util_2.h"
}

size_t OMoleGroup::_count = 0;

OMoleGroup::OMoleGroup(std::vector<FVector>& spawns, size_t tickRate) {
    _idx = _count;
    _tickRate = tickRate;
    for (auto& pos : spawns) {
        pos.x * xOrientation;
        OMole* ptr = reinterpret_cast<OMole*>(GetWorld()->AddObject(std::make_unique<OMole>(pos, this)));
        _moles.push_back({ptr, pos, false});
    }

    _count += 1;
}

void OMoleGroup::Tick() {
    for (size_t i = 0; i < std::min(_tickRate, _moles.size()); i++) {
        if (gObjectList[_moles[i].Mole->_objectIndex].state == 0) {
            OMoleGroup::func_80081FF4(_moles[i].Mole->_objectIndex);
        } else {
            _moles[i].Mole->func_800821AC(_moles[i].Mole->_objectIndex, 1);
        }
    }

    /**
     * This ticks the mole dirt particles. It must be ran *after* MoleGroup is done ticking. Otherwise, the dirt particle directions will not randomize
     * The best solution would be for particles to be its own class. But that takes effort
     * Instead, we wait until the last MoleGroup has ticked, then we tick the particles one time.
     * 
     * Warning: Calling this more than one time per frame, will result in doubling the speed
     */ 
    if (_idx == _count - 1) {
        for (size_t i = 0; i < gObjectParticle2_SIZE; i++) {
            s32 objectIndex = gObjectParticle2[i];
            if (gObjectList[objectIndex].state != 0) {
                if (nullptr != _moles[0].Mole) {
                    _moles[0].Mole->func_80081790(objectIndex);
                }
            }
        }
    }
}

void OMoleGroup::func_80081FF4(s32 objectIndex) {
    init_object(objectIndex, 0);
    gObjectList[objectIndex].unk_04C = random_int(30) + 5;

    s16 mole = random_int(_moles.size());
    for (size_t i = 0; i < _moles.size(); i++) {
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
