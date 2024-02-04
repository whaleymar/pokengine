#pragma once

#include "engine/effect.h"
#include "engine/move.h"
// TODO
namespace builder {

engine::Move getBasicMove() {
    return engine::Move(engine::ActionType::ATTACK, "Tackle", 1, engine::Type::NORMAL, 50, 0.95, 50, &engine::constant::NO_EFFECT,
                        engine::MoveAttributes::NONE, 0, true);
}
}  // namespace builder
