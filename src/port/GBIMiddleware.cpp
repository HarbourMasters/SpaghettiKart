#include <libultraship.h>

#include "Engine.h"
#include "DisplayList.h"
#include "resource/type/ResourceType.h"
#include "resource/type/Array.h"

extern "C" int GameEngine_OTRSigCheck(const char* data);

extern "C" void gSPDisplayList(Gfx* pkt, Gfx* dl) {
    char* imgData = (char*) dl;

    if (GameEngine_OTRSigCheck(imgData) == 1) {
        auto resource = Ship::Context::GetInstance()->GetResourceManager()->LoadResource(imgData);
        auto res = std::static_pointer_cast<LUS::DisplayList>(resource);
        dl = &res->Instructions[0];
    }

    __gSPDisplayList(pkt, dl);
}

extern "C" void gSPVertex(Gfx* pkt, uintptr_t v, int n, int v0) {

    if (GameEngine_OTRSigCheck((char*) v) == 1) {
        v = (uintptr_t) ResourceGetDataByName((char*) v);
    }

    __gSPVertex(pkt, v, n, v0);
}

extern "C" void gSPInvalidateTexCache(Gfx* pkt, uintptr_t texAddr) {
    auto data = reinterpret_cast<char*>(texAddr);

    if (texAddr != 0 && GameEngine_OTRSigCheck(data)) {
        const auto res = Ship::Context::GetInstance()->GetResourceManager()->LoadResource(data);
        const auto type = static_cast<LUS::ResourceType>(res->GetInitData()->Type);

        if (res->GetInitData()->Type == static_cast<uint32_t>(LUS::ResourceType::DisplayList)) {
            texAddr = reinterpret_cast<uintptr_t>(&std::static_pointer_cast<LUS::DisplayList>(res)->Instructions[0]);
        } else if (res->GetInitData()->Type == static_cast<uint32_t>(MK64::ResourceType::MK_Array)) {
            texAddr = reinterpret_cast<uintptr_t>(std::static_pointer_cast<MK64::Array>(res)->Vertices.data());
        } else {
            texAddr = reinterpret_cast<uintptr_t>(res->GetRawPointer());
        }
    }

    __gSPInvalidateTexCache(pkt, texAddr);
}
