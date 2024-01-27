#include "effect.h"
#include "battle.h"
#include "field.h"
#include "move.h"
#include "pokemon.h"

namespace engine {

Effect::Effect(EffectType type) : mType(type){};

EffectHolder::EffectHolder(Effect* effect, When when, bool canChange) : mEffect(effect), mWhen(when), mCanChange(canChange){};

StatEffect::StatEffect(Stat stat, s8 delta) : Effect(EffectType::STAT_BOOST), mStat(stat), mDelta(delta){};

void StatEffect::applyEffect(Battle* battle, Side side) const {
    battle->getTeam(side)->getActive()->boostStat(mStat, mDelta);
}

WeatherEffect::WeatherEffect(Weather weather) : Effect(EffectType::WEATHER_START), mWeather(weather){};

void WeatherEffect::applyEffect(Battle* battle, Side side) const {
    battle->getField()->startWeather(mWeather);
}

TerrainEffect::TerrainEffect(Terrain terrain) : Effect(EffectType::TERRAIN_START), mTerrain(terrain){};

void TerrainEffect::applyEffect(Battle* battle, Side side) const {
    battle->getField()->startTerrain(mTerrain);
}

SwitchEffect::SwitchEffect(bool isRandom) : Effect(EffectType::FORCE_SWITCH), mIsRandom(isRandom){};

void SwitchEffect::applyEffect(Battle* battle, Side side) const {
    if (mIsRandom) {
        auto switchOptions = battle->getTeam(side)->getSwitchActions();
        switchOptions[Battle::randint(0, switchOptions.size())]->execute(battle, side, side);
    } else {
        battle->requestAction(side, true, false)->execute(battle, side, side);
    }
}

SetHazardEffect::SetHazardEffect(Hazard hazard) : Effect(EffectType::SET_HAZARD), mHazard(hazard) {}

void SetHazardEffect::applyEffect(Battle* battle, Side side) const {
    battle->getField()->addHazard(mHazard, side);
}

SetBarrierEffect::SetBarrierEffect(Barrier barrier) : Effect(EffectType::SET_BARRIER), mBarrier(barrier){};

void SetBarrierEffect::applyEffect(Battle* battle, Side side) const {
    battle->getField()->addBarrier(mBarrier, side);
}

SetStatusEffect::SetStatusEffect(Status status) : Effect(EffectType::SET_STATUS), mStatus(status){};

void SetStatusEffect::applyEffect(Battle* battle, Side side) const {
    battle->getTeam(side)->getActive()->setStatus(mStatus);
}

SetVolatileStatusEffect::SetVolatileStatusEffect(VolatileStatus vStatus) : Effect(EffectType::SET_VSTATUS), mVStatus(vStatus){};

void SetVolatileStatusEffect::applyEffect(Battle* battle, Side side) const {
    Side otherSide = side == Side::PLAYER ? Side::OTHER : Side::PLAYER;
    battle->getTeam(side)->getActive()->addVolatileStatus(mVStatus, battle->getTeam(otherSide)->getActive());
}

SetItemEffect::SetItemEffect(Item* item) : Effect(EffectType::SET_ITEM), mItem(item){};

void SetItemEffect::applyEffect(Battle* battle, Side side) const {
    Side otherSide = side == Side::PLAYER ? Side::OTHER : Side::PLAYER;
    battle->getTeam(side)->getActive()->setItem(mItem);
}

SetPriorityEffect::SetPriorityEffect(s8 priority) : Effect(EffectType::SET_PRIORITY), mPriority(priority){};

void SetPriorityEffect::applyEffect(Battle* battle, Side side) const {
    battle->getTeam(side)->getActive()->setPriorityForNextMove(mPriority);
}

}  // namespace engine