#pragma once

#include <libultraship.h>
#include "World.h"

class Rulesets {
public:
    virtual void PreLoad();
    virtual void PreInit();
    virtual void PostInit();
};
