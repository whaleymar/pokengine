#include "ability.h"
#include "effect.h"

namespace engine {

Ability::Ability(const char* name, s32 ix, const Effect* effect, When when, bool canChange)
    : EffectHolder(effect, when, canChange), mName(name), mIx(ix){};
}  // namespace engine
