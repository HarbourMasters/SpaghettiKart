#include "ColPoly.h"

namespace SF64 {
ColPolyData* ColPoly::GetPointer() {
    return mColPolys.data();
}

size_t ColPoly::GetPointerSize() {
    return sizeof(mColPolys);
}
} // namespace SF64
