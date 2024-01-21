#pragma once

#include <iostream>
#include <string>
#include "dtypes.h"

namespace engine {

class BattlePokemon;
class Move;

class BattleAnnouncer {
public:
    static void usedMove(Move* move, BattlePokemon* attacker, BattlePokemon* defender);
    static void switchedOut(BattlePokemon* switcher);
    static void sentOut(BattlePokemon* switchee);

    static void failed();
    static void missed(BattlePokemon* defender);
    static void effectiveness(f32 multiplier);  // TODO arg

    static void log(std::string line) { std::cout << line << '\n'; }
};

}  // namespace engine