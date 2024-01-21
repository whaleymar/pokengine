#pragma once

#include "battle.h"
#include "dtypes.h"
#include "pokemon.h"
#include "type.h"

namespace engine {

#define SWITCH_PRIORITY 6
#define MOVE_COUNT 4

class BattleField;
class Team;
class Effect;

enum class ActionType { NONE, ATTACK, STATUS, SWITCH };

enum class MoveAttributes { NONE = 0, CONTACT = 1u << 0 };

class Action {
protected:
    ActionType mActionType = ActionType::NONE;
    s8 mPriority = 0;

public:
    Action(){};
    ~Action() = default;

    ActionType getActionType() { return mActionType; };
    s8 getPriority() { return mPriority; };
    virtual void execute(Battle* battle, Side source, Side target) = 0;
};

struct EffectPair {
    Effect* effect;
    f32 chance;
};

class Move : public Action {
private:
    const char* mName;
    const s32 mIx;
    Type mType;
    s32 mPower;
    f32 mAccuracy;
    s8 mPp;
    s8 mMaxPp;
    s8 mPriority;
    Effect* mPrimaryEffect;
    MoveAttributes mAttributes;
    bool mIsPhysical;
    std::vector<EffectPair*> mSecondaryEffects;

public:
    Move(const char* name, const s32 ix, Type type, s32 power, f32 accuracy, s8 maxpp, Effect* primaryEffect, MoveAttributes attributes,
         s8 priority = 0, bool isPhysical = false);
    ~Move();

    const char* getName();
    void addSecondaryEffect(Effect* effect, f32 probability);
    void resetPP();
    bool canUse();

    f32 calcDamage(Battle* battle, Side source, Side target);
    void execute(Battle* battle, Side source, Side target) override;
};

class Switch : public Action {
private:
    s8 mTargetIx;
    s8 mPriority = SWITCH_PRIORITY;

public:
    Switch(s8 mTargetIx);

    void execute(Battle* battle, Side source, Side target) override;
};

}  // namespace engine