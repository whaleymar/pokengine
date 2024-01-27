#include "battle.h"

#include <cstdlib>
#include <functional>
#include <random>
#include "ability.h"
#include "effect.h"
#include "field.h"
#include "move.h"
#include "pokemon.h"
#include "type.h"

namespace engine {

Team::Team() {
    mPokemon = new BattlePokemon*[6];
}

Team::~Team() {
    delete mPokemon;
}

s8 Team::getAliveCount() const {
    return mAliveCount;
}

BattlePokemon* Team::getActive() const {
    return mPokemon[mActive];
}

void Team::addPokemon(Pokemon* pokemon) {
    mPokemon[mSize] = new BattlePokemon(pokemon);
    mSize++;
}

bool Team::isReady() const {
    return mActive >= 0 && mPokemon[mActive] != nullptr && mPokemon[mActive]->isAlive();
}

bool Team::isValid() const {
    // check n pokemon and that each pokemon is valid TODO
    return mSize <= MAX_TEAM_SIZE;
}

void Team::faintPokemon() {
    if (~isReady() || !getActive()->isAlive()) {
        return;
    }
    mAliveCount--;
    mActive = -1;
}

void Team::revivePokemon(s8 ix) {
    if (mPokemon[ix]->isAlive()) {
        return;
    }
    mAliveCount++;
    mPokemon[ix]->setHpFraction(0.5);
}

void Team::switchPokemonOut(s8 ix) {
    if (!mAliveCount) {
        return;
    }
    getActive()->setInactive();
    mActive = ix;
    getActive()->setActive();
}

void Team::prepareForBattleStart(s8 startIx = 0) {
    for (s32 i = 0; i < mSize; i++) {
        mPokemon[i]->restore();
    }
    mActive = startIx;
    mAliveCount = mSize;
}

Action* Team::getSelectedAction() const {
    return mActionChoice;
}

void Team::setSelectedAction(Action* action) {
    mActionChoice = action;
}

s8 Team::getActionPriority() const {
    if (mActionChoice == nullptr || mActionChoice->getActionType() == ActionType::NONE) {
        return 0;
    }
    if (mActionChoice->getActionType() == ActionType::SWITCH) {
        return mActionChoice->getPriority();
    }
    if (getActive()->getPriorityForNextMove() != 0) {
        return getActive()->getPriorityForNextMove();
    }
    return mActionChoice->getPriority();
}

std::vector<Action*> Team::getSwitchActions() const {
    std::vector<Action*> actions;
    if (isReady() && mPokemon[mActive]->isTrapped()) {
        return actions;
    }
    for (s32 i = 0; i < mSize; i++) {
        if (mPokemon[i]->isAlive() && i != mActive) {
            actions.push_back(new Switch(i));
        }
    }
    return actions;
}

std::vector<Action*> Team::getMoveActions() const {
    std::vector<Action*> actions;
    if (!isReady()) {
        return actions;
    }
    for (s32 i = 0; i < MOVE_COUNT; i++) {
        if (mPokemon[mActive]->isMoveUsable(i)) {
            actions.push_back(mPokemon[mActive]->getMove(i));
        }
    }

    return actions;
}

std::vector<Action*> Team::getActionSet() const {
    std::vector<Action*> actions = getSwitchActions();
    if (!isReady()) {
        return actions;
    }

    std::vector<Action*> moveActions = getMoveActions();
    actions.insert(actions.end(), moveActions.begin(), moveActions.end());
    return actions;
}

Battle::Battle(Team* playerTeam, Team* otherTeam) : mTeamLeft(playerTeam), mTeamRight(otherTeam) {
    mField = new BattleField();
}

Battle::~Battle() {
    delete mField;
}

void Battle::doInOrder(const sidemethod& function, Side first) {
    if (first == Side::PLAYER) {
        (this->*function)(mTeamLeft, Side::PLAYER);
        (this->*function)(mTeamRight, Side::OTHER);
    } else {
        (this->*function)(mTeamRight, Side::OTHER);
        (this->*function)(mTeamLeft, Side::PLAYER);
    }
}

void Battle::applyEntranceEffects(Side side) {
    if (side == Side::ALL) {
        doInOrder(&Battle::applyEntranceEffectHazards, getFastestSide());
        doInOrder(&Battle::applyEntranceEffectAbility, getFastestSide());
        doInOrder(&Battle::applyEntranceEffectItem, getFastestSide());
        getTeam(Side::PLAYER)->getActive()->setEnteredBattle();
        getTeam(Side::OTHER)->getActive()->setEnteredBattle();

    } else {
        applyEntranceEffectHazards(getTeam(side), side);
        applyEntranceEffectAbility(getTeam(side), side);
        applyEntranceEffectItem(getTeam(side), side);
        getTeam(side)->getActive()->setEnteredBattle();
    }
}

void Battle::start() {
    mTeamLeft->prepareForBattleStart();
    mTeamRight->prepareForBattleStart();

    applyEntranceEffects();

    mTurnNo++;
    bool battleEnded = false;
    while (!battleEnded) {
        battleEnded = simulateTurn();
    }
}

void Battle::endTurn() {
    doInOrder(&Battle::applyEndOfTurnEffectAbility, getFastestSide());

    if (!mTeamLeft->isReady()) {
        requestAction(Side::PLAYER, true, false)->execute(this, Side::PLAYER, Side::PLAYER);
    } else {
        mTeamLeft->getActive()->setIsNotFirstTurn();
    }
    if (!mTeamRight->isReady()) {
        requestAction(Side::OTHER, true, false)->execute(this, Side::OTHER, Side::OTHER);
    } else {
        mTeamRight->getActive()->setIsNotFirstTurn();
    }

    applyEntranceEffects();
    mField->step();
    mTurnNo++;
}

bool Battle::simulateTurn() {
    Action* playerMove = requestAction(Side::PLAYER, !getTeam(Side::PLAYER)->getActive()->isTrapped(), true);
    Action* otherMove = requestAction(Side::OTHER, !getTeam(Side::OTHER)->getActive()->isTrapped(), true);
    getTeam(Side::PLAYER)->setSelectedAction(playerMove);
    getTeam(Side::OTHER)->setSelectedAction(otherMove);

    Side first;
    if (getTeam(Side::PLAYER)->getActionPriority() == getTeam(Side::OTHER)->getActionPriority()) {
        first = getFastestSide();
    } else {
        first = getTeam(Side::PLAYER)->getActionPriority() > getTeam(Side::OTHER)->getActionPriority() ? Side::PLAYER : Side::OTHER;
    }

    if (first == Side::PLAYER) {
        applyAction(playerMove, Side::PLAYER, Side::OTHER);
        if (isBattleOver()) {
            return true;
        }

        applyAction(otherMove, Side::OTHER, Side::PLAYER);
    } else {
        applyAction(otherMove, Side::OTHER, Side::PLAYER);
        if (isBattleOver()) {
            return true;
        }

        applyAction(playerMove, Side::PLAYER, Side::OTHER);
    }

    if (isBattleOver()) {
        return true;
    }

    endTurn();
    return isBattleOver();
}

bool Battle::isBattleOver() {
    return mTeamLeft->getAliveCount() == 0 || mTeamRight->getAliveCount() == 0;
}

Side Battle::getFastestSide() {
    s32 playerSpeed = mTeamLeft->getActive()->getStat(Stat::SPEED);
    s32 otherSpeed = mTeamRight->getActive()->getStat(Stat::SPEED);
    if (playerSpeed == otherSpeed) {
        if (randint(0, 2) == 0) {
            return Side::PLAYER;
        } else {
            return Side::OTHER;
        }
    }
    return playerSpeed > otherSpeed ? Side::PLAYER : Side::OTHER;
}

Team* Battle::getTeam(Side side) {
    if (side == Side::PLAYER) {
        return mTeamLeft;
    }
    return mTeamRight;
}

void Battle::applyEntranceEffectAbility(Team* team, Side side) {
    if (!team->getActive()->isEnteredBattle() || team->getActive()->getAbility()->getTiming() != When::ENTER) {
        return;
    }
    team->getActive()->getAbility()->getEffect()->applyEffect(this, side);
}

void Battle::applyEntranceEffectHazards(Team* team, Side side) {
    if (!team->getActive()->isEnteredBattle() || mField->getHazards(side)->isEmpty()) {
        return;
    }
    mField->getHazards(side)->applyEffects(team->getActive());
}

void Battle::applyEntranceEffectItem(Team* team, Side side) {
    if (!team->getActive()->isEnteredBattle() || team->getActive()->getItem() == nullptr) {
        return;
    }
    // TODO
}

void Battle::applyEndOfTurnEffectAbility(Team* team, Side side) {
    if (team->getActive()->getAbility()->getTiming() != When::STEP) {
        return;
    }
    team->getActive()->getAbility()->getEffect()->applyEffect(this, side);
}

void Battle::applyExitEffectAbility(Team* team, Side side) {
    if (team->getActive()->getAbility()->getTiming() != When::EXIT) {
        return;
    }
    team->getActive()->getAbility()->getEffect()->applyEffect(this, side);
}

void Battle::applyAction(Action* move, Side user, Side target) {
    /*
     * apply any effect that modifies the action
     * execute the action
     * apply entrance effects if switch
     * apply effects that happen after a move
     */
    // TODO would it make more sense for these to be in move.execute ? probably
    if (move->getActionType() == ActionType::ATTACK) {
        applyBeforeAttackEffectAbility(getTeam(user), user);
        applyBeforeAttackEffectItem(getTeam(user), user);

        applyBeforeDefendEffectAbility(getTeam(target), target);
        applyBeforeDefendEffectItem(getTeam(target), target);
    } else if (move->getActionType() == ActionType::SWITCH) {
    }

    move->execute(this, user, target);

    if (move->getActionType() == ActionType::ATTACK) {
        applyAfterDefendEffectAbility(getTeam(target), target);
        applyAfterDefendEffectItem(getTeam(target), target);

        if (getTeam(target)->isReady()) {
            applyAfterAttackEffectAbility(getTeam(user), user);
            applyAfterAttackEffectItem(getTeam(user), user);
        }
    } else if (move->getActionType() == ActionType::SWITCH) {
        applyEntranceEffects(user);
    }
}

BattleField* Battle::getField() {
    return mField;
}

Weather Battle::getWeather() {
    return mField->mWeather;
}

Barriers* Battle::getBarriers(Side side) {
    if (side == Side::PLAYER) {
        return mField->mBarriersLeft;
    }
    return mField->mBarriersRight;
}

Action* Battle::requestAction(Side side, bool canSwitch, bool canAttack) {
    // TODO
    // await some prompt from user
    // return std::Future<Action*>
    std::vector<Action*> actionSet = getTeam(side)->getActionSet();

    return actionSet[randint(0, actionSet.size())];  // TODO stubbed random action
}

}  // namespace engine