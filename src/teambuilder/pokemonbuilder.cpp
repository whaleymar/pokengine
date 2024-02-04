#include "pokemonbuilder.h"
#include <cstring>
#include "engine/ability.h"
#include "engine/move.h"

namespace builder {

void PokemonSpeciesBuilder::setName(const char* name) {
    strncpy(mName, name, MAX_NAME_LEN);
}

void PokemonSpeciesBuilder::setDexNo(s32 no) {
    mDexNo = no;
}

void PokemonSpeciesBuilder::setHp(s32 hp) {
    mHp = hp;
}

void PokemonSpeciesBuilder::setAttack(s32 attack) {
    mAttack = attack;
}

void PokemonSpeciesBuilder::setDefense(s32 defense) {
    mDefense = defense;
}

void PokemonSpeciesBuilder::setSpAttack(s32 spAttack) {
    mSpAttack = spAttack;
}

void PokemonSpeciesBuilder::setSpDefense(s32 spDefense) {
    mSpDefense = spDefense;
}

void PokemonSpeciesBuilder::setSpeed(s32 speed) {
    mSpeed = speed;
}

void PokemonSpeciesBuilder::setType(engine::Type type1, engine::Type type2) {
    mType1 = type1;
    mType2 = type2;
}

void PokemonSpeciesBuilder::setWeight(s32 weight) {
    mWeight = weight;
}

engine::PokemonSpecies PokemonSpeciesBuilder::build() {
    return engine::PokemonSpecies(mName, mDexNo, engine::Stats({mHp, mAttack, mDefense, mSpAttack, mSpDefense, mSpeed}), mType1, mType2, mWeight);
}

void PokemonSpeciesBuilder::reset() {
    strncpy(mName, "Bulbasaur", MAX_NAME_LEN);
    mDexNo = 1;
    mHp = 1;
    mAttack = 1;
    mDefense = 1;
    mSpAttack = 1;
    mSpDefense = 1;
    mSpeed = 1;
    mType1 = engine::Type::TYPELESS;
    mType2 = engine::Type::TYPELESS;
    mWeight = 1;
}

void PokemonBuilder::setSpecies(engine::PokemonSpecies* species) {
    mSpecies = species;
}
void PokemonBuilder::setLevel(s8 level) {
    mLevel = level;
}

void PokemonBuilder::setNature(engine::Nature nature) {
    mNature = nature;
}

void PokemonBuilder::setIv(engine::Stat stat, s32 iv) {
    switch (stat) {
    case engine::Stat::HP:
        mHpIv = iv;
        break;
    case engine::Stat::ATTACK:
        mAttackIv = iv;
        break;
    case engine::Stat::DEFENSE:
        mDefenseIv = iv;
        break;
    case engine::Stat::SPATTACK:
        mSpAttackIv = iv;
        break;
    case engine::Stat::SPDEFENSE:
        mSpDefenseIv = iv;
        break;
    case engine::Stat::SPEED:
        mSpeedIv = iv;
        break;
    }
}

void PokemonBuilder::setEv(engine::Stat stat, s32 ev) {
    switch (stat) {
    case engine::Stat::HP:
        mHpEv = ev;
        break;
    case engine::Stat::ATTACK:
        mAttackEv = ev;
        break;
    case engine::Stat::DEFENSE:
        mDefenseEv = ev;
        break;
    case engine::Stat::SPATTACK:
        mSpAttackEv = ev;
        break;
    case engine::Stat::SPDEFENSE:
        mSpDefenseEv = ev;
        break;
    case engine::Stat::SPEED:
        mSpeedEv = ev;
        break;
    }
}

void PokemonBuilder::setSex(engine::Sex sex) {
    mSex = sex;
}

void PokemonBuilder::setTeraType(engine::Type type) {
    mTeraType = type;
}
void PokemonBuilder::setMove1(engine::Move& move) {
    mMove1 = &move;
}
void PokemonBuilder::setMove2(engine::Move& move) {
    mMove2 = &move;
}
void PokemonBuilder::setMove3(engine::Move& move) {
    mMove3 = &move;
}
void PokemonBuilder::setMove4(engine::Move& move) {
    mMove4 = &move;
}
void PokemonBuilder::setAbility(engine::Ability* ability) {
    mAbility = ability;
}
void PokemonBuilder::setItem(engine::Item* item) {
    mItem = item;
}

engine::Pokemon PokemonBuilder::build() {
    engine::PokemonSpecies defaultSpecies = PokemonSpeciesBuilder().build();
    engine::PokemonSpecies* species;
    if (mSpecies == nullptr) {
        species = &defaultSpecies;
    } else {
        species = mSpecies;
    }
    return engine::Pokemon(species->mName.c_str(), species->mDexNo, species->mBaseStats, species->mType, engine::Type::TYPELESS, species->mWeight,
                           mLevel, mNature, engine::Stats({mHpIv, mAttackIv, mDefenseIv, mSpAttackIv, mSpDefenseIv, mSpeedIv}),
                           engine::Stats({mHpEv, mAttackEv, mDefenseEv, mSpAttackEv, mSpDefenseEv, mSpeedEv}), mAbility, mItem, mSex, mTeraType,
                           mMove1, mMove2, mMove3, mMove4);
}

void PokemonBuilder::reset() {
    mSpecies = nullptr;
    mLevel = 100;
    mNature = engine::Nature::SERIOUS;

    mHpIv = 31;
    mAttackIv = 31;
    mDefenseIv = 31;
    mSpAttackIv = 31;
    mSpDefenseIv = 31;
    mSpeedIv = 31;

    mHpEv = 0;
    mAttackEv = 0;
    mDefenseEv = 0;
    mSpAttackEv = 0;
    mSpDefenseEv = 0;
    mSpeedEv = 0;

    mSex = engine::Sex::NONE;
    mTeraType = engine::Type::NORMAL;

    mMove1 = nullptr;
    mMove2 = nullptr;
    mMove3 = nullptr;
    mMove4 = nullptr;

    mAbility = &engine::constant::NO_ABILITY;
    mItem = nullptr;
}
}  // namespace builder
