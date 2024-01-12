#pragma once

#include "dtypes.h"
#include "type.h"

namespace engine {

class Effect;
class BattlePokemon;

enum class Side { PLAYER, OTHER };

enum class Weather { NONE, RAIN, SUN, SAND, SNOW, RAINHEAVY, SUNHARSH };

enum class Terrain { NONE, ELECTRIC, PSYCHIC, GRASSY, MISTY };

enum class Barrier {
    NONE,
    REFLECT,
    LIGHTSCREEN,
    AURORAVEIL,
    SAFEGUARD
    // mist?
};

enum class Hazard { STONES, SPIKES, POISON_SPIKES, STEEL_SPIKES, STICK_WEB };
Type getHazardType(Hazard hazard);

class EntryHazards {
private:
    s8 mStoneCount = 0;
    s8 mSpikeCount = 0;
    s8 mPoisonSpikeCount = 0;
    s8 mSteelSpikeCount = 0;
    s8 mWebCount = 0;

public:
    EntryHazards();
    ~EntryHazards();
    void addHazard(Hazard hazard);
    void removeAllHazards();
    void removeToxicSpikes();
    bool isEmpty();
    void applyEffects(BattlePokemon* pokemon);
};

class Barriers {
private:
    s8 mReflectTurnsLeft = 0;
    s8 mLightScreenTurnsLeft = 0;
    s8 mAuroraVeilTurnsLeft = 0;

public:
    Barriers(){};
    void step();
    bool isBarrierActive(Barrier barrier);
    void setBarrier(Barrier barrier, bool longer);
};

class BattleField {
private:
    EntryHazards* mHazardsLeft;
    EntryHazards* mHazardsRight;
    Barriers* mBarriersLeft;
    Barriers* mBarriersRight;

    Weather mWeather = Weather::NONE;
    Terrain mTerrain = Terrain::NONE;
    s8 mWeatherTurnsLeft = 0;
    s8 mTerrainTurnsLeft = 0;
    bool mIsWeatherActive = false;

    friend class Battle;

public:
    BattleField();
    ~BattleField();

    void startWeather(Weather weather);
    void startTerrain(Terrain terrain);
    void addHazard(Hazard* hazard, Side side);
    EntryHazards* getHazards(Side side);
    void step();
};

}  // namespace engine