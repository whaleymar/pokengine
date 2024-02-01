#include "field.h"
#include "pokemon.h"
#include "type.h"
// TODO announce when things start, end, and fail
namespace engine {

HazardHolder::HazardHolder(Type type, s32 maxCount) : mType(type), mMaxCount(maxCount){};

void HazardHolder::tryAdd() {
    if (mCount < mMaxCount) {
        mCount++;
    }
}

void HazardHolder::clear() {
    mCount = 0;
}

bool HazardHolder::isEmpty() const {
    return mCount == 0;
}

void HazardHolder::tryApplyEffect(BattlePokemon* pokemon) {
    if (mCount == 0 || getEffectiveness(mType, pokemon->getType()) == Effectiveness::INEFFECTIVE) {
        return;
    }
    applyEffect(pokemon);
}

void HazardHolder::applyEffect(BattlePokemon* pokemon) {
    // apply 12.5% damage (typed)
    f32 damage = 0.125 * getEffectiveness(mType, pokemon->getType());
    pokemon->takeDamage(damage);
}

StealthRocks::StealthRocks() : HazardHolder(Type::ROCK, 1){};

Spikes::Spikes() : HazardHolder(Type::GROUND, 3){};

void Spikes::applyEffect(BattlePokemon* pokemon) {
    f32 damage;
    switch (mCount) {
    case 1:
        damage = 0.125;
    case 2:
        damage = 1 / 6;
    case 3:
        damage = 0.25;
    default:
        damage = 0.125;
    }
    pokemon->takeDamage(damage);
}

ToxicSpikes::ToxicSpikes() : HazardHolder(Type::POISON | Type::GROUND, 2){};

void ToxicSpikes::applyEffect(BattlePokemon* pokemon) {
    pokemon->setStatus(Status::PSN);
}

SteelSpikes::SteelSpikes() : HazardHolder(Type::STEEL | Type::GROUND, 1){};

StickyWeb::StickyWeb() : HazardHolder(Type::GROUND, 1){};

void StickyWeb::applyEffect(BattlePokemon* pokemon) {
    pokemon->boostStat(Stat::SPEED, -1);
}

Type getHazardType(Hazard hazard) {
    Type hazardTypes[] = {Type::ROCK, Type::GROUND, Type::POISON | Type::GROUND, Type::STEEL | Type::GROUND, Type::GROUND};
    return hazardTypes[static_cast<u32>(hazard)];
}

void EntryHazards::addHazard(Hazard hazard) {
    getHazard(hazard)->tryAdd();
}

void EntryHazards::removeAllHazards() {
    mRocks.clear();
    mSpikes.clear();
    mToxicSpikes.clear();
    mSteelSpikes.clear();
    mStickyWeb.clear();
}

bool EntryHazards::isEmpty() const {
    return mRocks.isEmpty() && mSpikes.isEmpty() && mToxicSpikes.isEmpty() && mSteelSpikes.isEmpty() && mStickyWeb.isEmpty();
}

void EntryHazards::applyEffects(BattlePokemon* pokemon) {
    mRocks.tryApplyEffect(pokemon);
    mSpikes.tryApplyEffect(pokemon);
    mToxicSpikes.tryApplyEffect(pokemon);
    mSteelSpikes.tryApplyEffect(pokemon);
    mStickyWeb.tryApplyEffect(pokemon);
}

void Barriers::step() {
    if (mReflectTurnsLeft > 0) {
        mReflectTurnsLeft--;
    }
    if (mLightScreenTurnsLeft > 0) {
        mLightScreenTurnsLeft--;
    }
    if (mAuroraVeilTurnsLeft > 0) {
        mAuroraVeilTurnsLeft--;
    }
}

void Barriers::clear() {
    mReflectTurnsLeft = 0;
    mLightScreenTurnsLeft = 0;
    mAuroraVeilTurnsLeft = 0;
}

bool Barriers::isBarrierActive(Barrier barrier) {
    switch (barrier) {
    case Barrier::REFLECT:
        return mReflectTurnsLeft > 0;
    case Barrier::LIGHTSCREEN:
        return mLightScreenTurnsLeft > 0;
    case Barrier::AURORAVEIL:
        return mAuroraVeilTurnsLeft > 0;
    default:
        return false;
    }
}

void Barriers::setBarrier(Barrier barrier, bool longer) {
    s8 nTurns = longer ? 8 : 5;
    switch (barrier) {
    case Barrier::REFLECT:
        if (mReflectTurnsLeft > 0) {
            return;
        }
        mReflectTurnsLeft = nTurns;
        break;
    case Barrier::LIGHTSCREEN:
        if (mLightScreenTurnsLeft > 0) {
            return;
        }
        mLightScreenTurnsLeft = nTurns;
        break;
    case Barrier::AURORAVEIL:
        if (mAuroraVeilTurnsLeft > 0) {
            return;
        }
        mAuroraVeilTurnsLeft = nTurns;
        break;
    default:
        break;
    }
}

BattleField::BattleField() {
    mHazardsLeft = new EntryHazards();
    mHazardsRight = new EntryHazards();
    mBarriersLeft = new Barriers();
    mBarriersRight = new Barriers();
}

BattleField::~BattleField() {
    delete mHazardsLeft;
    delete mHazardsRight;
    delete mBarriersLeft;
    delete mBarriersRight;
}

void BattleField::startWeather(Weather weather, bool longer) {
    mWeather = weather;
    mWeatherTurnsLeft = longer ? 8 : 5;
}

Weather BattleField::getWeather() const {
    return mWeather;
}

void BattleField::startTerrain(Terrain terrain) {
    mTerrain = terrain;
    mTerrainTurnsLeft = 5;
}

Terrain BattleField::getTerrain() const {
    return mTerrain;
}

void BattleField::addBarrier(Barrier barrier, Side side) {
    getBarriers(side)->setBarrier(barrier);
}

void BattleField::addHazard(Hazard hazard, Side side) {
    getHazards(side)->addHazard(hazard);
}

void BattleField::clearWeather() {
    mWeather = Weather::NONE;
    mWeatherTurnsLeft = 0;
}

void BattleField::clearTerrain() {
    mTerrain = Terrain::NONE;
    mTerrainTurnsLeft = 0;
}

void BattleField::clearHazards(Side side) {
    getHazards(side)->removeAllHazards();
}

void BattleField::clearBarriers(Side side) {
    getBarriers(side)->clear();
}

void BattleField::step() {
    mBarriersLeft->step();
    mBarriersRight->step();
    if (mWeatherTurnsLeft > 0) {
        if (mWeatherTurnsLeft == 1) {
            clearWeather();
        } else {
            mWeatherTurnsLeft--;
        }
    }
    if (mTerrainTurnsLeft > 0) {
        if (mTerrainTurnsLeft == 1) {
            clearTerrain();
        } else {
            mTerrainTurnsLeft--;
        }
    }
}
}  // namespace engine
