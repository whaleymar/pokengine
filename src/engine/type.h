#pragma once

#include <bitset>
#include "dtypes.h"
#include "enumops.h"

namespace engine {

enum class Type {
    TYPELESS = 0,
    NORMAL = (1u << 0),
    WATER = (1u << 1),
    FIRE = (1u << 2),
    GRASS = (1u << 3),
    FIGHTING = (1u << 4),
    ROCK = (1u << 5),
    GROUND = (1u << 6),
    STEEL = (1u << 7),
    GHOST = (1u << 8),
    DARK = (1u << 9),
    FAIRY = (1u << 10),
    POISON = (1u << 11),
    PSYCHIC = (1u << 12),
    FLYING = (1u << 13),
    ELECTRIC = (1u << 14),
    BUG = (1u << 15),
    DRAGON = (1u << 16),
    ICE = (1u << 17)
};

// could do macro for these (or maybe use inheritance)
// DEFINE_ENUM_OPS(Type); // not working
inline Type operator|(Type a, Type b) {
    return static_cast<Type>(static_cast<u32>(a) | static_cast<u32>(b));
}
inline Type& operator|=(Type& a, Type b) {
    return a = a | b;
}
inline Type operator&(Type a, Type b) {
    return static_cast<Type>(static_cast<u32>(a) & static_cast<u32>(b));
}
inline Type operator&=(Type a, Type b) {
    return a = a & b;
}
inline bool operator>(Type a, Type b) {
    return static_cast<u32>(a) > static_cast<u32>(b);
}
inline bool operator>(Type a, u32 b) {
    return static_cast<u32>(a) > b;
}
inline bool operator<(Type a, Type b) {
    return static_cast<u32>(a) < static_cast<u32>(b);
}
inline bool operator<(Type a, u32 b) {
    return static_cast<u32>(a) < b;
}
inline bool operator==(Type a, u32 b) {
    return static_cast<u32>(a) == b;
}

bool isType(Type fullType, Type type);

f32 getEffectiveness(Type source, Type target);

namespace Effectiveness {

const f32 INEFFECTIVE = 0.0;
const f32 NOT_VERY = 0.5;
const f32 NEUTRAL = 1.0;
const f32 SUPER = 2.0;
}  // namespace Effectiveness

}  // namespace engine