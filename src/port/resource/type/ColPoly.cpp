#include "ColPoly.h"

namespace SF64 {
ColPolyData* ColPoly::GetPointer() {
    return mColPolys.data();
}

size_t ColPoly::GetPointerSize() {
    return mColPolys.size() * sizeof(ColPolyData);
}
} // namespace SF64
