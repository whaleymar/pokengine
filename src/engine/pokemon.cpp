#include "pokemon.h"
#include "ability.h"
#include "effect.h"
#include "field.h"
#include "item.h"
#include "move.h"

namespace engine {

#define MAX_STAT_BOOST 6
#define MIN_STAT_BOOST -6

VolatileStatusTracker::VolatileStatusTracker(VolatileStatus status, BattlePokemon* sourceMon) {
    mStatus = status;
    mSourceMon = sourceMon;
}

PokemonSpecies::PokemonSpecies(const char* name, const s32 dexNo, const Stats basestats, const Type type1, const Type type2, const s32 weight)
    : mName(name), mDexNo(dexNo), mBaseStats(basestats), mType(type1 | type2), mWeight(weight) {}

Pokemon::Pokemon(const char* name, s32 dexNo, Stats basestats, Type type1, Type type2, s32 weight, s8 level, Nature nature, Stats ivs, Stats evs,
                 Ability* ability, Item* item, Sex sex, Type teraType, Move* move1, Move* move2, Move* move3, Move* move4)
    : mLevel(level), mNature(nature), mIvs(ivs), mEvs(evs), mAbility(ability), mItem(item), mSex(sex), mTeraType(teraType),
      PokemonSpecies(name, dexNo, basestats, type1, type2, weight) {
    calcStats();
    mMoves = new Move* [4] { move1, move2, move3, move4 };
}

Pokemon::~Pokemon() {
    delete mMoves;
}

f32 Pokemon::getNatureBoost(Nature nature, Stat stat) {
    return BOOST_TABLE[(s32)stat - 1][(s32)nature];
}

s32 Pokemon::calcNonHpStat(s32 base, s32 iv, s32 ev, s32 level, f32 natureBoost) {
    return static_cast<s32>(static_cast<f32>((static_cast<s32>((2 * base + iv + static_cast<s32>(static_cast<f32>(ev) / 4)) * level / 100) + 5)) *
                            natureBoost);
}

void Pokemon::calcStats() {
    mStats.hp = static_cast<s32>((2 * mBaseStats.hp + mIvs.hp + static_cast<s32>(static_cast<f32>(mEvs.hp) / 4)) * mLevel / 100) + mLevel + 10;
    mStats.attack = calcNonHpStat(mBaseStats.attack, mIvs.attack, mEvs.attack, mLevel, Pokemon::getNatureBoost(mNature, Stat::ATTACK));
    mStats.defense = calcNonHpStat(mBaseStats.defense, mIvs.defense, mEvs.defense, mLevel, Pokemon::getNatureBoost(mNature, Stat::DEFENSE));
    mStats.spattack = calcNonHpStat(mBaseStats.spattack, mIvs.spattack, mEvs.spattack, mLevel, Pokemon::getNatureBoost(mNature, Stat::SPATTACK));
    mStats.spdefense = calcNonHpStat(mBaseStats.spdefense, mIvs.spdefense, mEvs.spdefense, mLevel, Pokemon::getNatureBoost(mNature, Stat::SPDEFENSE));
    mStats.speed = calcNonHpStat(mBaseStats.speed, mIvs.speed, mEvs.speed, mLevel, Pokemon::getNatureBoost(mNature, Stat::SPEED));
}

BattlePokemon::BattlePokemon(Pokemon* pokemon) : mPokemon(pokemon) {
    mHp = mPokemon->mStats.hp;
    mActiveType = mPokemon->mType;
    mActiveAbility = pokemon->mAbility;
    mActiveItem = pokemon->mItem;
    mVolatileStatuses = std::vector<VolatileStatusTracker*>();
    if (isType(mActiveType, Type::FLYING) || hasVolatileStatus(VolatileStatus::AIRBORNE)) {  // TODO make sure levitate applies this status
        mIsGrounded = false;
    } else {
        mIsGrounded = true;
    }
}

BattlePokemon::~BattlePokemon() {
    resetVolatileEffects();
}

void BattlePokemon::resetVolatileEffects() {
    for (s32 i = 0; i < mVolatileStatuses.size(); i++) {
        delete mVolatileStatuses[i];
    }
    mVolatileStatuses.clear();
    mBoosts = Stats();
}

s32 BattlePokemon::getStat(Stat statIx) const {
    switch (statIx) {
    case Stat::HP:
        return mPokemon->mStats.hp;
    case Stat::ATTACK:
        return mPokemon->mStats.attack * getStatBoostModifier(true, getStatBoostLevel(Stat::ATTACK));
    case Stat::DEFENSE:
        return mPokemon->mStats.defense * getStatBoostModifier(true, getStatBoostLevel(Stat::DEFENSE));
    case Stat::SPATTACK:
        return mPokemon->mStats.spattack * getStatBoostModifier(true, getStatBoostLevel(Stat::SPATTACK));
    case Stat::SPDEFENSE:
        return mPokemon->mStats.spdefense * getStatBoostModifier(true, getStatBoostLevel(Stat::SPDEFENSE));
    case Stat::SPEED:
        return mPokemon->mStats.speed * getStatBoostModifier(true, getStatBoostLevel(Stat::SPEED));
    }
}

s8 BattlePokemon::getStatBoostLevel(Stat statIx) const {
    switch (statIx) {
    case Stat::HP:
        return 0;
    case Stat::ATTACK:
        return mBoosts.attack;
    case Stat::DEFENSE:
        return mBoosts.defense;
    case Stat::SPATTACK:
        return mBoosts.spattack;
    case Stat::SPDEFENSE:
        return mBoosts.spdefense;
    case Stat::SPEED:
        return mBoosts.speed;
    }
}

f32 BattlePokemon::getStatBoostModifier(bool notAccuracyEvasion, s8 boostLevel) const {
    if (notAccuracyEvasion) {
        return STAT_MODIFIER_NORMAL[boostLevel + 6];
    } else {
        return STAT_MODIFIER_ACCURACY[boostLevel + 6];
    }
}

void BattlePokemon::setStatBoost(Stat statIx, s8 boostLevel) {
    switch (statIx) {
    case Stat::HP:
        break;
    case Stat::ATTACK:
        mBoosts.attack = boostLevel;
        break;
    case Stat::DEFENSE:
        mBoosts.defense = boostLevel;
        break;
    case Stat::SPATTACK:
        mBoosts.spattack = boostLevel;
        break;
    case Stat::SPDEFENSE:
        mBoosts.spdefense = boostLevel;
        break;
    case Stat::SPEED:
        mBoosts.speed = boostLevel;
        break;
    }
}

void BattlePokemon::boostStat(Stat statIx, s8 nStages) {
    s8 newLevel = getStatBoostLevel(statIx) + nStages;
    if (newLevel >= MAX_STAT_BOOST) {
        setStatBoost(statIx, MAX_STAT_BOOST);
    } else if (newLevel <= MIN_STAT_BOOST) {
        setStatBoost(statIx, MIN_STAT_BOOST);
    } else {
        setStatBoost(statIx, newLevel);
    }
}

void BattlePokemon::setType(Type type) {
    mActiveType = type;
}

void BattlePokemon::resetType() {
    mActiveType = mPokemon->mType;
}

void BattlePokemon::resetAbility() {
    mActiveAbility = mPokemon->mAbility;
}

f32 BattlePokemon::getHpFraction() const {
    // might need explicit cast
    return mHp / mPokemon->mStats.hp;
}

void BattlePokemon::setHpFraction(f32 fraction) {
    if (fraction > 1) {
        fraction = 1.0;
    }
    mHp = static_cast<s32>(fraction * mPokemon->mStats.hp);
}

void BattlePokemon::takeDamage(s32 damage) {
    if (damage >= mHp) {
        mHp = 0;
        return;
    }
    mHp -= damage;
}

void BattlePokemon::takeDamage(f32 hpFraction) {
    f32 curHpFraction = getHpFraction();
    if (hpFraction >= curHpFraction) {
        mHp = 0;
        return;
    }
    setHpFraction(curHpFraction - hpFraction);
}

void BattlePokemon::healDamage(s32 hp) {
    s32 newHp = hp + mHp;
    if (mPokemon->mStats.hp >= newHp) {
        mHp = mPokemon->mStats.hp;
        return;
    }
    mHp = newHp;
}

void BattlePokemon::healDamage(f32 hpFraction) {
    takeDamage(-hpFraction);
}

void BattlePokemon::restore() {
    mHp = mPokemon->mStats.hp;
    mActiveType = mPokemon->mType;
    resetVolatileEffects();
    for (s32 i = 0; i < MOVE_COUNT; i++) {
        mPokemon->mMoves[i]->resetPP();
    }
    mActiveItem = mPokemon->mItem;
    mActiveAbility = mPokemon->mAbility;
}

bool BattlePokemon::isAlive() const {
    return mHp > 0;
}

bool BattlePokemon::isTrapped() const {
    // TODO check for arena trap, shadow tag, magnet pull (PUT THIS IN VSTATUS SETTER / FUNCTION THAT RUNS WHEN OPPONENT IS SENT OUT)
    // for (VolatileStatusTracker* vStatusHolder : mVolatileStatuses) {
    //     if (vStatusHolder->mStatus == VolatileStatus::BOUND) {
    //         return true;
    //     }
    // }
    // return false;
    return mIsTrapped;
}

Ability* BattlePokemon::getAbility() const {
    return mActiveAbility;
}

bool BattlePokemon::isGrounded() const {
    return mIsGrounded;
}

void BattlePokemon::setActive() {
    mIsFirstTurn = true;
}

void BattlePokemon::setInactive() {
    resetVolatileEffects();
    if (!mIsTerastallized) {
        resetType();
    }
    resetAbility();
}

bool BattlePokemon::isMoveUsable(s8 moveIx) const {
    return mPokemon->mMoves[moveIx]->canUse();
}

Move* BattlePokemon::getMove(s8 moveIx) const {
    return mPokemon->mMoves[moveIx];
}

Item* BattlePokemon::getItem() const {
    return mActiveItem;
}

bool BattlePokemon::isFirstTurn() const {
    return mIsFirstTurn;
}

void BattlePokemon::setIsNotFirstTurn() {
    mIsFirstTurn = false;
}

Status BattlePokemon::getStatus() const {
    return mStatus;
}

void BattlePokemon::setStatus(Status status) {
    mStatus = status;
}

bool BattlePokemon::hasVolatileStatus(VolatileStatus vStatus) const {
    for (VolatileStatusTracker* vStatusTracker : mVolatileStatuses) {
        if (vStatusTracker->mStatus == vStatus) {
            return true;
        }
    }
    return false;
}

void BattlePokemon::addVolatileStatus(VolatileStatus vStatus, BattlePokemon* sourceMon) {
    VolatileStatusTracker* vStatusTracker = new VolatileStatusTracker(vStatus, sourceMon);
    mVolatileStatuses.push_back(vStatusTracker);
}

}  // namespace engine