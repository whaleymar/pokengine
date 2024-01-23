#include "item.h"

namespace engine {

Item::Item(char* name, s32 ix, Effect* effect, When when, bool canChange) : EffectHolder(effect, when, canChange), mName(name), mItemIx(ix){};

}