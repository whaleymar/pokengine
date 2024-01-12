#pragma once

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

enum class MoveAttributes { PHYSICAL = 1u << 0, SPECIAL = 1u << 1, CONTACT = 1u << 2 };

class Action {
private:
    ActionType mActionType = ActionType::NONE;
    s8 mPriority = 0;

public:
    Action(){};
    ~Action() = default;

    ActionType getType() { return mActionType; };
    s8 getPriority() { return mPriority; };
    virtual void execute(BattleField* field, Team* sourceTeam, Team* targetTeam) = 0;
};

class Move : public Action {
private:
    s32 mIx;
    Type mType;
    s32 mPower;
    f32 mAccuracy;
    s8 mPp;
    s8 mMaxPp;
    s8 mPriority;
    Effect* mPrimaryEffect;
    Effect* mSecondaryEffect;
    f32 mSecondaryEffectChance;
    MoveAttributes mAttributes;

public:
    Move(s32 ix, Type type, s32 power, f32 accuracy, s8 maxpp, Effect primaryEffect, Effect secondaryEffect, f32 secondaryEffectChance,
         s8 priority = 0);
    ~Move();

    void resetPP();
    bool canUse();
    void execute(BattleField* field, Team* sourceTeam, Team* targetTeam) override;
};

class Switch : public Action {
private:
    s8 mTargetIx;
    s8 mPriority = SWITCH_PRIORITY;

public:
    Switch(s8 mTargetIx);

    void execute(BattleField* field, Team* sourceTeam, Team* targetTeam) override;
};

}  // namespace engine