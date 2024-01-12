#pragma once

#include <functional>
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

    void applyEntranceEffectAbility(Team* team, Side side);
    void applyEntranceEffectHazards(Team* team, Side side);
    void applyEntranceEffectItem(Team* team, Side side);  // TODO (air ballon, terrain seeds)
    void applyEntranceEffects();

    void applyEndOfTurnEffectAbility(Team* team, Side side);

    void applyExitEffectAbility(Team* team, Side side);

public:
    Battle(Team* playerTeam, Team* otherTeam);
    ~Battle();

    void doInOrder(const sidemethod& function, Side first);
    void start();
    void endTurn();
    bool simulateTurn(Action* playerMove, Action* otherMove);  // ret true if battle ended
    bool isBattleOver();
    Side getFastestSide();

    s8 requestAction(Side side);
};

}  // namespace engine