#include "libultra_internal.h"

OSThread* __osGetCurrFaultedThread() {
    return __osActiveQueue; // 80302efc
}
