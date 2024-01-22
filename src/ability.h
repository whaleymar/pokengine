#pragma once

#include "dtypes.h"

namespace engine {

class BattleField;
class Effect;
enum class When;
enum class Side;
class Team;

class Ability {  // TODO
private:
    const char* mName;
    const s32 mIx;
    Effect* mEffect;
    When mWhen;
    bool mCanChange;

public:
    Ability(char* name, s32 ix, bool canChange = true);
    ~Ability() = default;

    Effect* getEffect();
    When getTiming();
};

}  // namespace engine