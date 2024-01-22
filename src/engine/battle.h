#pragma once

#include <functional>
#include <iostream>
#include <random>
#include <vector>
#include "dtypes.h"
#include "field.h"

#define MAX_TEAM_SIZE 6

namespace engine {

class Pokemon;
class BattlePokemon;
class Action;
class Move;
class Switch;

class Team {
private:
    BattlePokemon** mPokemon;
    s8 mActive;
    s8 mAliveCount = 0;
    s8 mSize = 0;
    bool mIsTerastallized = false;

public:
    Team();
    ~Team();

    s8 getAliveCount() const;
    BattlePokemon* getActive() const;

    void addPokemon(Pokemon* pokemon);
    bool isReady() const;
    bool isValid() const;
    void faintPokemon();
    void revivePokemon(s8 ix);
    void switchPokemonOut(s8 ix);
    void prepareForBattleStart(s8 startIx);
    std::vector<Action*> getSwitchActions() const;
    std::vector<Action*> getMoveActions() const;
    std::vector<Action*> getActionSet() const;
};

class Battle;
using sidemethod = void (engine::Battle::*)(Team*, Side);

class Battle {
private:
    Team* mTeamLeft;
    Team* mTeamRight;
    BattleField* mField;
    s32 mTurnNo = 0;
    std::default_random_engine mGenerator;
    std::uniform_real_distribution<f32> mDistribution;

    void applyEntranceEffectAbility(Team* team, Side side);  // (intimidate)
    void applyEntranceEffectHazards(Team* team, Side side);  //
    void applyEntranceEffectItem(Team* team, Side side);     // TODO (air ballon, terrain seeds)
    void applyEntranceEffects();                             // calls ^

    void applyEndOfTurnEffectAbility(Team* team, Side side);  // (speed boost, slow start)
    void applyEndOfTurnEffectItem(Team* team, Side side);     // TODO (leftovers)

    void applyExitEffectAbility(Team* team, Side side);  // (unnerve)

    // TODO should pass the attack to these
    void applyBeforeAttackEffectAbility(Team* team, Side side);  // TODO (protean)
    void applyBeforeAttackEffectItem(Team* team, Side side);     // TODO
    void applyAfterAttackEffectAbility(Team* team, Side side);   // TODO
    void applyAfterAttackEffectItem(Team* team, Side side);      // TODO (life orb)

    void applyBeforeDefendEffectAbility(Team* team, Side side);  // TODO
    void applyBeforeDefendEffectItem(Team* team, Side side);     // TODO (shuca berry)
    void applyAfterDefendEffectAbility(Team* team, Side side);   // TODO (stamina, mummy)
    void applyAfterDefendEffectItem(Team* team, Side side);      // TODO

    void applyAction(Action* move, Side user, Side target);

public:
    Battle(Team* playerTeam, Team* otherTeam);
    ~Battle();

    void doInOrder(const sidemethod& function, Side first);
    void start();
    void endTurn();
    bool simulateTurn(Action* playerMove, Action* otherMove);  // ret true if battle ended
    bool isBattleOver();
    Side getFastestSide();
    Team* getTeam(Side side);
    Weather getWeather();
    BattleField* getField();
    Barriers* getBarriers(Side side);
    Action* requestAction(Side side, bool canSwitch, bool canAttack);

    f32 sample() { return mDistribution(mGenerator); };
    bool roll(f32 probability) {
        if (probability == 1.0) {
            return true;
        }
        return probability >= sample();
    };
    s32 randint(s32 min, s32 max) {
        // returns random int between min and max (exclusive) // TODO make sure is exclusive
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(mGenerator);
    }
};

}  // namespace engine