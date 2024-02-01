#pragma once

#include "dtypes.h"
#include "type.h"

namespace engine {

class Effect;
class BattlePokemon;

enum class Side { PLAYER, OTHER, ALL };

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

// i could store hazards in a class with a type and an Effect* but I don't want to pass a Battle* to the applyEffect so I'm duplicating some code
enum class Hazard { STONES, SPIKES, POISON_SPIKES, STEEL_SPIKES, STICK_WEB };

constexpr s32 NUM_HAZARDS = 5;  // macro?

using HazardEffect = void(BattlePokemon*, Type);

class HazardHolder {
protected:
    Type mType;
    s32 mCount = 0;
    s32 mMaxCount;

public:
    HazardHolder(Type type, s32 maxCount);
    ~HazardHolder() = default;

    void tryAdd();
    void clear();
    bool isEmpty() const;
    void tryApplyEffect(BattlePokemon* pokemon);
    virtual void applyEffect(BattlePokemon* pokemon);
};

class StealthRocks : public HazardHolder {
public:
    StealthRocks();
};

class Spikes : public HazardHolder {
public:
    Spikes();
    void applyEffect(BattlePokemon* pokemon) override;
};

class ToxicSpikes : public HazardHolder {
public:
    ToxicSpikes();
    void applyEffect(BattlePokemon* pokemon) override;
};

class SteelSpikes : public HazardHolder {
public:
    SteelSpikes();
};

class StickyWeb : public HazardHolder {
public:
    StickyWeb();
    void applyEffect(BattlePokemon* pokemon) override;
};

class EntryHazards {
private:
    StealthRocks mRocks;
    Spikes mSpikes;
    ToxicSpikes mToxicSpikes;
    SteelSpikes mSteelSpikes;
    StickyWeb mStickyWeb;

    HazardHolder* getHazard(Hazard hazard) {
        switch (hazard) {
        case Hazard::STONES:
            return &mRocks;
        case Hazard::SPIKES:
            return &mSpikes;
        case Hazard::POISON_SPIKES:
            return &mToxicSpikes;
        case Hazard::STEEL_SPIKES:
            return &mSteelSpikes;
        case Hazard::STICK_WEB:
            return &mStickyWeb;
        }
        return nullptr;
    }

public:
    EntryHazards() = default;
    ~EntryHazards() = default;
    void addHazard(Hazard hazard);
    void removeAllHazards();
    bool isEmpty() const;
    void applyEffects(BattlePokemon* pokemon);
};

class Barriers {
private:
    s8 mReflectTurnsLeft = 0;
    s8 mLightScreenTurnsLeft = 0;
    s8 mAuroraVeilTurnsLeft = 0;

public:
    Barriers() = default;
    ~Barriers() = default;

    void clear();
    void step();
    bool isBarrierActive(Barrier barrier);
    void setBarrier(Barrier barrier, bool longer = false);
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

    friend class Battle;

    EntryHazards* getHazards(Side side) {
        if (side == Side::PLAYER) {
            return mHazardsLeft;
        }
        return mHazardsRight;
    }
    Barriers* getBarriers(Side side) {
        if (side == Side::PLAYER) {
            return mBarriersLeft;
        }
        return mBarriersRight;
    }

public:
    BattleField();
    ~BattleField();

    void startWeather(Weather weather, bool longer = false);
    Weather getWeather() const;
    void startTerrain(Terrain terrain);
    Terrain getTerrain() const;
    void addBarrier(Barrier barrier, Side side);
    void addHazard(Hazard hazard, Side side);
    void clearWeather();
    void clearTerrain();
    void clearHazards(Side side);
    void clearBarriers(Side side);

    void step();
};

}  // namespace engine
