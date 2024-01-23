#pragma once

#include "dtypes.h"
#include "effect.h"

namespace engine {

class Item : public EffectHolder {
private:
    const char* mName;
    const s32 mItemIx;

public:
    Item(char* name, s32 ix, Effect* effect, When when, bool canChange = true);
    ~Item() = default;
};

}  // namespace engine