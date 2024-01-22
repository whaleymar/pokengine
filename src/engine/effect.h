#pragma once

#include <vector>
#include "dtypes.h"
#include "field.h"

namespace engine {

class BattleField;
enum class Side;
enum class Stat;
class Team;

enum class EffectType {
    // STAT_CHANGE, vstatus
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
    BOOST_ATTACK,  // sheer force
    BOOST_DEFENSE  // filter
};

enum class When { ENTER, EXIT, ATTACK_BEFORE, ATTACK_AFTER, DEFEND_BEFORE, DEFEND_AFTER, STEP };

class Effect {
private:
    EffectType mType;

public:
    Effect(EffectType type);
    ~Effect() = default;

    virtual void applyEffect(BattleField* field, Side side, Team* team) = 0;
};

class StatEffect : public Effect {
private:
    Stat mStat;
    s8 mDelta;

public:
    StatEffect(Stat stat, s8 delta);
    void applyEffect(BattleField* field, Side side, Team* team) override;
};

class WeatherEffect : public Effect {
private:
    Weather mWeather;

public:
    WeatherEffect(Weather weather);  // : Effect(EffectType::WEATHER_START) { mWeather = weather; }
    void applyEffect(BattleField* field, Side side, Team* team) override;
};

class TerrainEffect : public Effect {
private:
    Terrain mTerrain;

public:
    TerrainEffect(Terrain terrain);
    void applyEffect(BattleField* field, Side side, Team* team) override;
};

class SwitchEffect : public Effect {
private:
    s8 mTeamIx;

public:
    SwitchEffect(s8 teamIx);
    void applyEffect(BattleField* field, Side side, Team* team) override;
};

class SetHazardEffect : public Effect {
private:
    Hazard mHazard;

public:
    SetHazardEffect(Hazard hazard);
    void applyEffect(BattleField* field, Side side, Team* team) override;
};

}  // namespace engine