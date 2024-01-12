#pragma once

#include <vector>
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
 * some need numbers attached (sub health, turns left)
 * some are attached to an opposing pokemon
 * some remain until a different pokemon leaves
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

enum class Sex { NONE, M, F };

class PokemonSpecies {
private:
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

    Stats mStats;
    Ability* mAbility;
    const Ability* mAbilityOriginal;
    Item* mItem;
    const Item* mItemOriginal;

    void calcStats();

    friend class BattlePokemon;

public:
    Pokemon(const char* name, s32 dexNo, Stats basestats, Type type1, Type type2, s32 weight, s8 level, Nature nature, Stats ivs, Stats evs,
            Ability* ability, Item* item, Sex sex, Type teraType);
};

class BattlePokemon {
private:
    Pokemon* mPokemon;
    Move** moves;
    s32 mHp;
    Stats mBoosts = Stats();
    Status mStatus = Status::NONE;
    s8 mStatusTurns;
    VolatileStatus mVolatileStatus = VolatileStatus::NONE;  // TODO should be vector of statuses
    s8 mVolatileStatusTurns;
    Type mActiveType;
    s8 mLastUsedMoveIx;
    bool mIsTerastallized = false;
    bool mIsGrounded;
    bool mIsActive;  // unsure if needed
    bool mIsChoiceLocked = false;
    bool mIsFirstTurn = false;

public:
    BattlePokemon(Pokemon* pokemon);
    ~BattlePokemon();

    void resetVolatileEffects();
    s32 getStat(Stat statIx);
    void setType(Type type);
    void resetType();
    f32 getHpFraction();
    void setHpFraction(f32 fraction);
    void unalive();
    void restore();
    bool isAlive();
    bool isTrapped();
    Ability* getAbility();
    bool applyEndOfTurnEffects();  // TODO make this a Battle method. Effects dont all happen to one poke then other poke. Is actually poisoncheck
                                   // poke1 poisoncheck poke2, terrainheal poke1 terrainheal poke2, etc
    bool isGrounded();
    void setActive();  // should set mIsFirstTurn = true
    void setInactive();
    bool isMoveUsable(s8 moveIx);
    Move* getMove(s8 moveIx);
    Item* getItem();
    bool isFirstTurn();
    void setIsNotFirstTurn();
};

}  // namespace engine