#include "Message.h"

namespace SF64 {
void* Message::GetPointer() {
    return mMessage.data();
}

size_t Message::GetPointerSize() {
    return mMessage.size() * sizeof(uint16_t);
}

MsgLookup* MessageLookup::GetPointer() {
    return mLookupTable.data();
}

size_t MessageLookup::GetPointerSize() {
    return mLookupTable.size() * sizeof(MsgLookup);
}
} // namespace SF64
