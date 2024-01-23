#pragma once

#include "dtypes.h"
#include "effect.h"

namespace engine {

class BattleField;
class Effect;
enum class When;
enum class Side;
class Team;

class Ability : public EffectHolder {
private:
    const char* mName;
    const s32 mIx;

public:
    Ability(char* name, s32 ix, Effect* effect, When when, bool canChange = true);
    ~Ability() = default;
};

}  // namespace engine