#include "pokemon.h"
#include "ability.h"
#include "item.h"
#include "move.h"

namespace engine {

PokemonSpecies::PokemonSpecies(const char* name, const s32 dexNo, const Stats basestats, const Type type1, const Type type2, const s32 weight)
    : mName(name), mDexNo(dexNo), mBaseStats(basestats), mType(type1 | type2), mWeight(weight) {}

Pokemon::Pokemon(const char* name, s32 dexNo, Stats basestats, Type type1, Type type2, s32 weight, s8 level, Nature nature, Stats ivs, Stats evs,
                 Ability* ability, Item* item, Sex sex, Type teraType, Move* move1, Move* move2, Move* move3, Move* move4)
    : mLevel(level), mNature(nature), mIvs(ivs), mEvs(evs), mAbility(ability), mAbilityOriginal(ability), mItem(item), mItemOriginal(item), mSex(sex),
      mTeraType(teraType), PokemonSpecies(name, dexNo, basestats, type1, type2, weight) {
    calcStats(false);
    mMoves = new Move*[4];
    mMoves[0] = move1;
    mMoves[1] = move2;
    mMoves[2] = move3;
    mMoves[3] = move4;
}

Pokemon::~Pokemon() {
    delete mMoves;
}

void Pokemon::calcStats(bool ignoreHp) {}  // TODO

BattlePokemon::BattlePokemon(Pokemon* pokemon) : mPokemon(pokemon) {
    mHp = mPokemon->mStats.hp;
    mActiveType = mPokemon->mType;
    mVolatileStatuses = std::vector<VolatileStatusTracker*>();
    if (isType(mActiveType, Type::FLYING)) {  // TODO check levitate
        mIsGrounded = false;
    } else {
        mIsGrounded = true;
    }
}

BattlePokemon::~BattlePokemon() {
    resetVolatileEffects();
}

void BattlePokemon::resetVolatileEffects() {
    // idk if i will actually dynamically allocate these...
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
        return mPokemon->mStats.attack;
    case Stat::DEFENSE:
        return mPokemon->mStats.defense;
    case Stat::SPATTACK:
        return mPokemon->mStats.spattack;
    case Stat::SPDEFENSE:
        return mPokemon->mStats.spdefense;
    case Stat::SPEED:
        return mPokemon->mStats.speed;
    }
}

void BattlePokemon::setType(Type type) {
    mActiveType = type;
}

void BattlePokemon::resetType() {
    mActiveType = mPokemon->mType;
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
    // TODO move these to Pokemon class
    // mPokemon->mItem = mPokemon->mItemOriginal;
    // mPokemon->mAbility = mPokemon->mAbilityOriginal;
}

}  // namespace engine