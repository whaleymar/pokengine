#include "move.h"
#include <format>
#include "effect.h"
#include "log.h"
#include "type.h"

namespace engine {

MoveBase::MoveBase(ActionType actionType, const char* name, const s32 ix, Type type, s32 power, f32 accuracy, s8 maxpp, const Effect* primaryEffect,
                   MoveAttributes attributes, s8 priority, bool isPhysical)
    : mName(name), mIx(ix), mType(type), mPower(power), mAccuracy(accuracy), mMaxPp(maxpp), mPrimaryEffect(primaryEffect), mAttributes(attributes),
      mIsPhysical(isPhysical) {
    mActionType = actionType;
    mPriority = priority;
    mSecondaryEffects = std::vector<EffectPair*>();
}

MoveBase::~MoveBase() {
    for (u64 i = 0; i < mSecondaryEffects.size(); i++) {
        delete mSecondaryEffects[i];
    }
}

const char* MoveBase::getName() {
    return mName;
}

void MoveBase::addSecondaryEffect(Effect* effect, f32 probability) {
    mSecondaryEffects.push_back(new EffectPair({effect, probability}));
}

f32 MoveBase::calcDamage(Battle* battle, Side source, Side target) {
    // these should maybe all be ints TODO

    if (mActionType == ActionType::STATUS) {
        return 0.0;
    }

    BattlePokemon* sourceMon = battle->getTeam(source)->getActive();
    BattlePokemon* targetMon = battle->getTeam(target)->getActive();

    f32 typeMultiplier = getEffectiveness(mType, targetMon->getType());
    if (typeMultiplier == Effectiveness::INEFFECTIVE) {
        BattleAnnouncer::effectiveness(typeMultiplier);
        return 0.0;
    }

    s32 attack = mIsPhysical ? sourceMon->getStat(Stat::ATTACK) : sourceMon->getStat(Stat::SPATTACK);
    s32 defense = mIsPhysical ? targetMon->getStat(Stat::DEFENSE) : targetMon->getStat(Stat::SPDEFENSE);

    // TODO effective move power (burn, last respects)
    f32 weatherMult = 1.0;
    if (mType == Type::WATER && (battle->getWeather() == Weather::RAIN || battle->getWeather() == Weather::RAINHEAVY)) {
        weatherMult = 1.5;
    } else if (mType == Type::WATER && (battle->getWeather() == Weather::SUN || battle->getWeather() == Weather::SUNHARSH)) {
        weatherMult = 0.5;  // might be 0 for harsh sun idk
    } else if (mType == Type::FIRE && (battle->getWeather() == Weather::SUN || battle->getWeather() == Weather::SUNHARSH)) {
        weatherMult = 1.5;
    } else if (mType == Type::FIRE && (battle->getWeather() == Weather::RAIN || battle->getWeather() == Weather::RAINHEAVY)) {
        weatherMult = 0.5;  // same comment as above
    }

    // sample crit here since screens are dependent on if it's a crit
    bool isCrit = false;  // TODO

    f32 brnMult = 1.0;
    f32 screenMult = 1.0;
    if (mIsPhysical) {
        if (sourceMon->getStatus() == Status::BRN) {
            // TODO guts
            brnMult = 0.5;
        }
        if (!isCrit &&
            (battle->getBarriers(target)->isBarrierActive(Barrier::REFLECT) || battle->getBarriers(target)->isBarrierActive(Barrier::AURORAVEIL))) {
            screenMult = 0.5;
        }
    } else {
        if (!isCrit && (battle->getBarriers(target)->isBarrierActive(Barrier::LIGHTSCREEN) ||
                        (battle->getBarriers(target)->isBarrierActive(Barrier::AURORAVEIL)))) {
            screenMult = 0.5;
        }
    }

    // ability multipliers like flash fire go here TODO
    f32 damage = (((2 * sourceMon->getLevel() / 5) + 2) * mPower * attack / defense / 50 + 2) * weatherMult * screenMult * brnMult;

    if (isCrit) {
        // TODO sniper ability
        damage *= 1.5;
    }

    // TODO item checks go here, apparently only life orb and metronome are relevant

    f32 randomness = battle->sample() * 0.15 + 0.85;
    damage *= randomness;

    // STAB // TODO adaptability
    if (isType(sourceMon->getType(), mType)) {
        damage *= 1.5;
    }

    // TODO need to check abilities & if grounded
    damage *= typeMultiplier;
    BattleAnnouncer::effectiveness(typeMultiplier);
    // there are a bunch of other modifiers here, see bulbapedia

    return damage;
}

Move::Move(ActionType actionType, const char* name, const s32 ix, Type type, s32 power, f32 accuracy, s8 maxpp, const Effect* primaryEffect,
           MoveAttributes attributes, s8 priority, bool isPhysical)
    : MoveBase(actionType, name, ix, type, power, accuracy, maxpp, primaryEffect, attributes, priority, isPhysical), mPp(maxpp) {}

void Move::resetPP() {
    mPp = mMaxPp;
}

bool Move::canUse() {
    return mPp > 0 && !mIsDisabled;
}

void Move::execute(Battle* battle, Side source, Side target) {
    BattleAnnouncer::usedMove(this, battle->getTeam(source)->getActive(), battle->getTeam(target)->getActive());
    if (!battle->roll(mAccuracy)) {
        BattleAnnouncer::missed(battle->getTeam(target)->getActive());
        return;
    }
    BattlePokemon* targetMon = battle->getTeam(target)->getActive();

    if (mActionType == ActionType::ATTACK) {
        s32 damage = static_cast<s32>(calcDamage(battle, source, target));
        if (damage) {
            targetMon->takeDamage(damage);
        }
        BattleAnnouncer::log(std::format("Did {} damage", damage));
        BattleAnnouncer::logHealth(targetMon);
    } else {  // ActionType::STATUS
        mPrimaryEffect->applyEffect(battle, target);
    }

    for (EffectPair* effectPair : mSecondaryEffects) {
        if (battle->roll(effectPair->chance)) {
            effectPair->effect->applyEffect(battle, target);
        }
    }

    mPp--;
}

Switch::Switch(s8 targetIx) : mTargetIx(targetIx) {
    mActionType = ActionType::SWITCH;
    mPriority = SWITCH_PRIORITY;
};

void Switch::execute(Battle* battle, Side source, Side target) {
    // source and target are the same

    // not sure if I should check if trapped here. for now I'll assume I can switch
    Team* team = battle->getTeam(source);
    team->switchPokemonOut(mTargetIx);
    BattleAnnouncer::sentOut(team->getActive());
}

}  // namespace engine
