#pragma once

#include <vector>
#include "dtypes.h"
#include "field.h"

namespace engine {

class Battle;
class Item;
enum class Side;
enum class Stat;
enum class Status;
enum class VolatileStatus;

enum class EffectType {
    STAT_BOOST,  // could do set_vstatus? not sure tbh
    WEATHER_START,
    TERRAIN_START,
    FORCE_SWITCH,
    SET_HAZARD,
    SET_BARRIER,
    SET_STATUS,
    SET_VSTATUS,
    SET_ITEM,
    SET_PRIORITY,
    APPLY_DAMAGE,
    APPLY_HEALING,
    // TODO rethink this approach
    BOOST_ATTACK,    // sheer force / pure power`
    BOOST_DEFENSE,   // filter
    BOOST_SPDEFENSE  // ice scales
};

// these will get very complicated for some abilities. rough skin, sheer force, ice scales, filter -- a lot to think about
// rough skin is DEFEND_AFTER, but with added contact move requirement
// sheer force is ATTACK_BEFORE, but with secondary effect requirement (and removes the effects)
// ice scales is DEFEND_BEFORE, but checks whether the attacking move is physical or special
// filter is DEFEND_BEFORE, but checks if incoming move is supereffective.
// I think the solution is to store conditions in an EffectHolder which are checked in addition to the When member
enum class When { ENTER, EXIT, ATTACK_BEFORE, ATTACK_AFTER, DEFEND_BEFORE, DEFEND_AFTER, STEP, MANUAL };

class Effect {
private:
    EffectType mType;

public:
    Effect(EffectType type);
    ~Effect() = default;

    // virtual void applyEffect(BattleField* field, Side side, Team* team) const = 0;
    virtual void applyEffect(Battle* battle, Side side) const = 0;
};

class EffectHolder {
private:
    const Effect* mEffect;
    const When mWhen;
    const bool mCanChange;

public:
    EffectHolder(Effect* effect, When when, bool canChange);
    ~EffectHolder() = default;

    const Effect* getEffect() const { return mEffect; };
    When getTiming() const { return mWhen; };
    bool isChangeable() const { return mCanChange; };
};

class NoEffect : public Effect {
public:
    NoEffect();
    void applyEffect(Battle* battle, Side side) const override{};
};

class StatEffect : public Effect {
private:
    Stat mStat;
    s8 mDelta;

public:
    StatEffect(Stat stat, s8 delta);
    void applyEffect(Battle* battle, Side side) const override;
};

class WeatherEffect : public Effect {
private:
    Weather mWeather;

public:
    WeatherEffect(Weather weather);
    void applyEffect(Battle* battle, Side side) const override;
};

class TerrainEffect : public Effect {
private:
    Terrain mTerrain;

public:
    TerrainEffect(Terrain terrain);
    void applyEffect(Battle* battle, Side side) const override;
};

class SwitchEffect : public Effect {
private:
    bool mIsRandom;

public:
    SwitchEffect(bool isRandom);
    void applyEffect(Battle* battle, Side side) const override;
};

class SetHazardEffect : public Effect {
private:
    Hazard mHazard;

public:
    SetHazardEffect(Hazard hazard);
    void applyEffect(Battle* battle, Side side) const override;
};

class SetBarrierEffect : public Effect {
private:
    Barrier mBarrier;

public:
    SetBarrierEffect(Barrier barrier);
    void applyEffect(Battle* battle, Side side) const override;
};

class SetStatusEffect : public Effect {
private:
    Status mStatus;

public:
    SetStatusEffect(Status status);
    void applyEffect(Battle* battle, Side side) const override;
};

class SetVolatileStatusEffect : public Effect {
private:
    VolatileStatus mVStatus;

public:
    SetVolatileStatusEffect(VolatileStatus vStatus);
    void applyEffect(Battle* battle, Side side) const override;
};

class SetItemEffect : public Effect {
private:
    Item* mItem;

public:
    SetItemEffect(Item* item);
    void applyEffect(Battle* battle, Side side) const override;
};

class SetPriorityEffect : public Effect {
private:
    s8 mPriority;

public:
    SetPriorityEffect(s8 priority);
    void applyEffect(Battle* battle, Side side) const override;
};

class SetDamageEffect : public Effect {
private:
    f32 mDamage;

public:
    SetDamageEffect(f32 damage);
    void applyEffect(Battle* battle, Side side) const override;
};

class SetHealingEffect : public Effect {
private:
    f32 mHealAmount;

public:
    SetHealingEffect(f32 healAmount);
    void applyEffect(Battle* battle, Side side) const override;
};

// I don't like this approach
// class SetAttackBoostEffect : public Effect {
// private:
//    f32 mBoostAmount;
//
// public:
//    SetAttackBoostEffect(f32 boostAmount);
//    void applyEffect(Battle* battle, Side side) const override;
//};

}  // namespace engine
