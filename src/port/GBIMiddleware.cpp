#include <libultraship.h>

#include "Engine.h"
#include "DisplayList.h"
#include "resource/type/ResourceType.h"
#include "resource/type/Array.h"
extern "C" {
#include <align_asset_macro.h>
}

extern "C" void gSPDisplayList(Gfx* pkt, Gfx* dl) {
    char* imgData = (char*) dl;

    if (GameEngine_OTRSigCheck(imgData)) {
        auto resource = Ship::Context::GetInstance()->GetResourceManager()->LoadResource(imgData);
        auto res = std::static_pointer_cast<Fast::DisplayList>(resource);
        dl = &res->Instructions[0];
    }

    __gSPDisplayList(pkt, dl);
}

extern "C" void gSPVertex(Gfx* pkt, uintptr_t v, int n, int v0) {

    if (GameEngine_OTRSigCheck((char*) v)) {
        v = (uintptr_t) ResourceGetDataByName((char*) v);
    }

    __gSPVertex(pkt, v, n, v0);
}

extern "C" void gSPInvalidateTexCache(Gfx* pkt, uintptr_t texAddr) {
#ifdef __SWITCH__
    // TODO: This kills performance on the Switch, we need to limit the amount of times we call this
    return;
#endif
    auto data = reinterpret_cast<char*>(texAddr);

    if (texAddr != 0 && GameEngine_OTRSigCheck(data)) {
        const auto res = Ship::Context::GetInstance()->GetResourceManager()->LoadResource(data);
        const auto type = static_cast<Fast::ResourceType>(res->GetInitData()->Type);

        if (res->GetInitData()->Type == static_cast<uint32_t>(Fast::ResourceType::DisplayList)) {
            texAddr = reinterpret_cast<uintptr_t>(&std::static_pointer_cast<Fast::DisplayList>(res)->Instructions[0]);
        } else if (res->GetInitData()->Type == static_cast<uint32_t>(MK64::ResourceType::MK_Array)) {
            texAddr = reinterpret_cast<uintptr_t>(std::static_pointer_cast<MK64::Array>(res)->Vertices.data());
        } else {
            texAddr = reinterpret_cast<uintptr_t>(res->GetRawPointer());
        }
    }

    __gSPInvalidateTexCache(pkt, texAddr);
}
