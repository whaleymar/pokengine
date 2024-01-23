#pragma once

#include <vector>
#include "dtypes.h"
#include "field.h"

namespace engine {

class Battle;
enum class Side;
enum class Stat;

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
    BOOST_ATTACK,    // sheer force / pure power`
    BOOST_DEFENSE,   // filter
    BOOST_SPDEFENSE  // ice scales
};

// these will get very complicated for some abilities. rough skin, sheer force, ice scales, filter -- a lot to think about
// rough skin is DEFEND_AFTER, but with added contact move requirement
// sheer force is ATTACK_BEFORE, but with secondary effect requirement (and removes the effects)
// ice scales is DEFEND_BEFORE, but checks whether the attacking move is physical or special
// filter is DEFEND_BEFORE, but checks if incoming move is supereffective.
enum class When { ENTER, EXIT, ATTACK_BEFORE, ATTACK_AFTER, DEFEND_BEFORE, DEFEND_AFTER, STEP };

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

}  // namespace engine