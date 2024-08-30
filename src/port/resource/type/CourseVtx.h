#pragma once

#include "resource/Resource.h"
#include <vector>
#include <libultra/gbi.h>

typedef struct {
    s16 ob[3]; /* x, y, z */
    s16 tc[2]; /* texture coord */
    s8 ca[4];  /* color & alpha */
} CourseVtxData;

namespace MK64 {
class CourseVtxClass : public Ship::Resource<CourseVtxData> {
  public:
    using Resource::Resource;

    CourseVtxClass();

    CourseVtxData* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<CourseVtxData> CourseVtxList;
};
} // namespace MK64
