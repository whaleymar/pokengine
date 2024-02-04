#include "log.h"
#include <format>
#include "move.h"
#include "pokemon.h"
#include "type.h"

namespace engine {

void BattleAnnouncer::usedMove(Move* move, BattlePokemon* attacker, BattlePokemon* defender) {
    std::string line = std::string(attacker->getName()) + " used " + move->getName();
    log(line);
}

void BattleAnnouncer::switchedOut(BattlePokemon* switcher) {
    log(std::string(switcher->getName()) + " switched out");
}

void BattleAnnouncer::sentOut(BattlePokemon* switchee) {
    log(std::string(switchee->getName()) + " switched in");
}

void BattleAnnouncer::fainted(BattlePokemon* pokemon) {
    log(std::string(pokemon->getName()) + " fainted");
}

void BattleAnnouncer::logHealth(BattlePokemon* pokemon) {
    log(std::format("{}/{}", pokemon->getCurrentHp(), pokemon->getStat(Stat::HP)));
}

void BattleAnnouncer::logWinner(Side side) {
    std::string str;
    if (side == Side::ALL) {
        str = "Tie";
    } else if (side == Side::PLAYER) {
        str = "Player 1 wins";
    } else {
        str = "Player 2 wins";
    }
    log("Battle Result: " + str);
}
void BattleAnnouncer::failed() {
    log("But it failed");
}

void BattleAnnouncer::missed(BattlePokemon* defender) {
    log(std::string(defender->getName()) + " avoided the attack");
}

void BattleAnnouncer::effectiveness(f32 multiplier) {
    if (multiplier == Effectiveness::NEUTRAL) {
        return;
    }

    if (multiplier == Effectiveness::INEFFECTIVE) {
        log("It's ineffective");
        return;
    }

    std::string str;
    if (multiplier == Effectiveness::SUPER) {
        str = "Super";
    } else {
        str = "Not Very";
    }

    log("It's " + str + " Effective");
};
}  // namespace engine
