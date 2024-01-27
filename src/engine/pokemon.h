#pragma once

#include <vector>
#include "battle.h"
#include "dtypes.h"
#include "item.h"
#include "type.h"

namespace engine {

class Ability;
class Move;
class Effect;

enum class Stat { HP = 0, ATTACK = 1, DEFENSE = 2, SPATTACK = 3, SPDEFENSE = 4, SPEED = 5 };

const f32 STAT_MODIFIER_NORMAL[13] = {0.25, 2 / 7, 1 / 3, 0.4, 0.5, 2 / 3, 1, 1.5, 2, 2.5, 3, 3.5, 4};
const f32 STAT_MODIFIER_ACCURACY[13] = {1 / 3, 0.375, 3 / 7, 0.5, 0.6, 0.75, 1, 4 / 3, 5 / 3, 2, 7 / 3, 8 / 3, 3};

struct Stats {
    s32 hp = 0;
    s32 attack = 0;
    s32 defense = 0;
    s32 spattack = 0;
    s32 spdefense = 0;
    s32 speed = 0;
};

enum class Nature {
    ADAMANT,
    BASHFUL,
    BOLD,
    BRAVE,
    CALM,
    CAREFUL,
    DOCILE,
    GENTLE,
    HARDY,
    HASTY,
    IMPISH,
    JOLLY,
    LAX,
    LONELY,
    MILD,
    MODEST,
    NAIVE,
    NAUGHTY,
    QUIET,
    QUIRKY,
    RASH,
    RELAXED,
    SASSY,
    SERIOUS,
    TIMID
};

const f32 BOOST_TABLE[5][25] = {
    {1.1, 1.0, 0.9, 1.1, 0.9, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.1, 1.0, 0.9, 1.0, 1.1, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.9},
    {1.0, 1.0, 1.1, 1.0, 1.0, 1.0, 1.0, 0.9, 1.0, 0.9, 1.1, 1.0, 1.1, 0.9, 0.9, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.1, 1.0, 1.0, 1.0},
    {0.9, 1.0, 1.0, 1.0, 1.0, 0.9, 1.0, 1.0, 1.0, 1.0, 0.9, 0.9, 1.0, 1.0, 1.1, 1.1, 1.0, 1.0, 1.1, 1.0, 1.1, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.1, 1.1, 1.0, 1.1, 1.0, 1.0, 1.0, 1.0, 0.9, 1.0, 1.0, 1.0, 0.9, 0.9, 1.0, 1.0, 0.9, 1.0, 1.1, 1.0, 1.0},
    {1.0, 1.0, 1.0, 0.9, 1.0, 1.0, 1.0, 1.0, 1.0, 1.1, 1.0, 1.1, 1.0, 1.0, 1.0, 1.0, 1.1, 1.0, 0.9, 1.0, 1.0, 0.9, 0.9, 1.0, 1.1}};

enum class Status { NONE, PAR, BRN, FRZ, SLP, PSN, TOX };

/*
 * these are kind of complex
 * some need numbers attached (substitute health, # turns left)
 * some are attached to an opposing pokemon
 * some remain until the inflicting pokemon leaves
 * most remain until the affected pokemon leaves
 */
enum class VolatileStatus {
    NONE,
    CONFUSE,
    LOVE,
    CURSE,
    ENCORE,  // use with choice lock?
    DISABLE,
    SUBSTITUTE,
    ABILITY_CHANGE,
    TYPE_CHANGE,
    TRANSFORM,
    BOUND,
    NIGHTMARE,
    PERISH_SONG,
    SEEDED,
    SALT_CURE,
    MINIMIZED,
    TAR,
    GROUNDED,
    AIRBORNE,
    HEAL_TURN_END,  // aqua ring, ingrain
    // AQUA_RING,
    // INGRAIN,
    CRIT_FOCUS,
    TAKING_AIM,
    DROWSY,
    CHARGED,
    STOCKPILE,
    NO_ESCAPE,
    EMBARGO,
    HEAL_BLOCK,
    IMPRISON,
    TAUNT,
    TORMENT,
    THROAT_CHOP,
    // TODO several more are left
};

class BattlePokemon;

class VolatileStatusTracker {
private:
    VolatileStatus mStatus;
    BattlePokemon* mSourceMon;
    s32 mHp;
    s32 mTurns = 0;

    friend class BattlePokemon;

public:
    VolatileStatusTracker(VolatileStatus status, BattlePokemon* sourceMon);
    ~VolatileStatusTracker() = default;
};

enum class Sex { NONE, M, F };

class PokemonSpecies {
protected:
    const char* mName;
    const s32 mDexNo;
    const Stats mBaseStats;
    const Type mType;
    const s32 mWeight;

public:
    PokemonSpecies(const char* name, const s32 dexNo, const Stats basestats, const Type type1, const Type type2, const s32 weight);
};

class Pokemon : PokemonSpecies {
private:
    const s8 mLevel;
    const Nature mNature;
    const Stats mIvs;
    const Stats mEvs;
    const Sex mSex;
    const Type mTeraType;

    Move** mMoves;
    Stats mStats;
    Ability* mAbility;
    Item* mItem;

    static f32 getNatureBoost(Nature nature, Stat stat);
    static s32 calcNonHpStat(s32 base, s32 iv, s32 ev, s32 level, f32 natureBoost);
    void calcStats();

    friend class BattlePokemon;

public:
    Pokemon(const char* name, s32 dexNo, Stats basestats, Type type1, Type type2, s32 weight, s8 level, Nature nature, Stats ivs, Stats evs,
            Ability* ability, Item* item, Sex sex, Type teraType, Move* move1, Move* move2, Move* move3, Move* move4);
    ~Pokemon();
};

class BattlePokemon {
private:
    const Pokemon* mPokemon;
    s32 mHp;
    Stats mBoosts = Stats();
    Status mStatus = Status::NONE;
    s8 mStatusTurns = 0;
    std::vector<VolatileStatusTracker*> mVolatileStatuses;
    Type mActiveType;
    Ability* mActiveAbility;
    Item* mActiveItem;
    s8 mLastUsedMoveIx = -1;
    s8 mNextMovePriority = 0;
    bool mIsTerastallized = false;
    bool mIsGrounded;
    bool mIsTrapped = false;
    bool mIsEnteredBattle = false;  // used to check if we need to apply entrance effects
    bool mIsChoiceLocked = false;
    bool mIsFirstTurn = false;

    void setStatBoost(Stat statIx, s8 boostLevel);

public:
    BattlePokemon(Pokemon* pokemon);
    ~BattlePokemon();

    void resetVolatileEffects();
    s32 getStat(Stat statIx) const;
    s8 getStatBoostLevel(Stat statIx) const;
    f32 getStatBoostModifier(bool notAccuracyEvasion, s8 boostLevel) const;
    void boostStat(Stat statIx, s8 nStages);
    Type getType() { return mActiveType; };
    void setType(Type type);
    void resetType();
    void resetAbility();
    f32 getHpFraction() const;
    void setHpFraction(f32 fraction);
    void takeDamage(s32 damage);
    void takeDamage(f32 hpFraction);
    void healDamage(s32 hp);
    void healDamage(f32 hpFraction);
    void restore();
    bool isAlive() const;
    bool isTrapped() const;
    Ability* getAbility() const;
    bool isGrounded() const;
    void setActive();
    void setInactive();
    void setEnteredBattle();
    bool isEnteredBattle() const;
    bool isMoveUsable(s8 moveIx) const;
    Move* getMove(s8 moveIx) const;
    Item* getItem() const;
    void setItem(Item* item);
    bool hasItem() const;
    bool isFirstTurn() const;
    void setIsNotFirstTurn();
    Status getStatus() const;
    void setStatus(Status status);
    bool hasVolatileStatus(VolatileStatus vStatus) const;
    void addVolatileStatus(VolatileStatus vStatus, BattlePokemon* sourceMon);
    void setPriorityForNextMove(s8 priority);
    s8 getPriorityForNextMove() const;

    s8 getLevel() { return mPokemon->mLevel; };
    const char* getName() { return mPokemon->mName; };
};

}  // namespace engine