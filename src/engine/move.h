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
    //    ~Action() = default;

    ActionType getActionType() { return mActionType; };
    s8 getPriority() { return mPriority; };
    virtual void execute(Battle* battle, Side source, Side target) = 0;
};

struct EffectPair {
    Effect* effect;
    f32 chance;
};

class MoveBase : public Action {
protected:
    const char* mName;
    const s32 mIx;
    Type mType;
    s32 mPower;
    f32 mAccuracy;
    s8 mMaxPp;
    const Effect* mPrimaryEffect;
    MoveAttributes mAttributes;
    bool mIsPhysical;
    std::vector<EffectPair*> mSecondaryEffects;

public:
    MoveBase(ActionType actionType, const char* name, const s32 ix, Type type, s32 power, f32 accuracy, s8 maxpp, const Effect* primaryEffect,
             MoveAttributes attributes, s8 priority, bool isPhysical);
    ~MoveBase();

    const char* getName();
    void addSecondaryEffect(Effect* effect, f32 probability);
    f32 calcDamage(Battle* battle, Side source, Side target);
};

class Move : public MoveBase {
private:
    s8 mPp;
    bool mIsDisabled = false;

public:
    Move(ActionType actionType, const char* name, const s32 ix, Type type, s32 power, f32 accuracy, s8 maxpp, const Effect* primaryEffect,
         MoveAttributes attributes, s8 priority = 0, bool isPhysical = false);

    void resetPP();
    bool canUse();
    void execute(Battle* battle, Side source, Side target) override;
};

class Switch : public Action {
private:
    s8 mTargetIx;

public:
    Switch(s8 targetIx);

    void execute(Battle* battle, Side source, Side target) override;
};

}  // namespace engine
