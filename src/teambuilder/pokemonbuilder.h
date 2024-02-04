#pragma once

#include "basebuilder.h"
#include "engine/ability.h"
#include "engine/pokemon.h"
#include "engine/type.h"

namespace builder {
#define MAX_NAME_LEN 64

class PokemonSpeciesBuilder : public Builder<engine::PokemonSpecies> {
private:
    char mName[MAX_NAME_LEN] = "Bulbasaur";
    s32 mDexNo = 1;
    s32 mHp = 1;
    s32 mAttack = 1;
    s32 mDefense = 1;
    s32 mSpAttack = 1;
    s32 mSpDefense = 1;
    s32 mSpeed = 1;
    engine::Type mType1 = engine::Type::TYPELESS;
    engine::Type mType2 = engine::Type::TYPELESS;
    s32 mWeight = 1;

public:
    void setName(const char* name);
    void setDexNo(s32 no);
    void setHp(s32 hp);
    void setAttack(s32 attack);
    void setDefense(s32 defense);
    void setSpAttack(s32 spAttack);
    void setSpDefense(s32 spDefense);
    void setSpeed(s32 speed);
    void setType(engine::Type type1, engine::Type type2 = engine::Type::TYPELESS);
    void setWeight(s32 weight);

    engine::PokemonSpecies build() override;
    void reset() override;
};

class PokemonBuilder : public Builder<engine::Pokemon> {
private:
    engine::PokemonSpecies* mSpecies = nullptr;
    s8 mLevel = 100;
    engine::Nature mNature = engine::Nature::SERIOUS;

    s32 mHpIv = 31;
    s32 mAttackIv = 31;
    s32 mDefenseIv = 31;
    s32 mSpAttackIv = 31;
    s32 mSpDefenseIv = 31;
    s32 mSpeedIv = 31;

    s32 mHpEv = 0;
    s32 mAttackEv = 0;
    s32 mDefenseEv = 0;
    s32 mSpAttackEv = 0;
    s32 mSpDefenseEv = 0;
    s32 mSpeedEv = 0;

    engine::Sex mSex = engine::Sex::NONE;
    engine::Type mTeraType = engine::Type::NORMAL;

    // TODO if i'm allowing these to be null I need null checking in their methods
    engine::Move* mMove1 = nullptr;
    engine::Move* mMove2 = nullptr;
    engine::Move* mMove3 = nullptr;
    engine::Move* mMove4 = nullptr;

    const engine::Ability* mAbility = &engine::constant::NO_ABILITY;
    engine::Item* mItem = nullptr;

public:
    void setSpecies(engine::PokemonSpecies* species);
    void setLevel(s8 level);
    void setNature(engine::Nature nature);
    void setIv(engine::Stat stat, s32 iv);
    void setEv(engine::Stat stat, s32 ev);
    void setSex(engine::Sex sex);
    void setTeraType(engine::Type type);
    void setMove1(engine::Move& move);
    void setMove2(engine::Move& move);
    void setMove3(engine::Move& move);
    void setMove4(engine::Move& move);
    void setAbility(engine::Ability* ability);
    void setItem(engine::Item* item);

    engine::Pokemon build() override;
    void reset() override;
};
}  // namespace builder
