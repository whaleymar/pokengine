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

struct Stats {
    s32 hp = 0;
    s32 attack = 0;
    s32 defense = 0;
    s32 spattack = 0;
    s32 spdefense = 0;
    s32 speed = 0;
};

enum class Nature;  // TODO

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

public:
    VolatileStatusTracker(VolatileStatus status, BattlePokemon* pokemon);
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
    Ability* const mAbilityOriginal;
    Item* mItem;
    Item* const mItemOriginal;

    void calcStats(bool ignoreHp = true);

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
    s8 mLastUsedMoveIx = -1;
    bool mIsTerastallized = false;
    bool mIsGrounded;
    bool mIsActive = false;  // unsure if needed
    bool mIsChoiceLocked = false;
    bool mIsFirstTurn = false;

public:
    BattlePokemon(Pokemon* pokemon);
    ~BattlePokemon();

    void resetVolatileEffects();
    s32 getStat(Stat statIx) const;
    void setType(Type type);
    void resetType();
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
    bool applyEndOfTurnEffects();  // TODO make this a Battle method. Effects dont all happen to one poke then other poke. Is actually poisoncheck
                                   // poke1 poisoncheck poke2, terrainheal poke1 terrainheal poke2, etc
    bool isGrounded() const;
    void setActive();  // should set mIsFirstTurn = true
    void setInactive();
    bool isMoveUsable(s8 moveIx) const;
    Move* getMove(s8 moveIx) const;
    Item* getItem() const;
    bool isFirstTurn() const;
    void setIsNotFirstTurn();
    Status getStatus() const;

    s8 getLevel() { return mPokemon->mLevel; };
    Type getType() { return mActiveType; };
    const char* getName() { return mPokemon->mName; };
};

}  // namespace engine