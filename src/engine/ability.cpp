#include "ability.h"
#include "effect.h"

namespace engine {

Ability::Ability(char* name, s32 ix, Effect* effect, When when, bool canChange) : EffectHolder(effect, when, canChange), mName(name), mIx(ix){};
}  // namespace engine