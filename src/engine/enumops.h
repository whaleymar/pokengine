#pragma once
#include <bitset>
#include "dtypes.h"

#define DEFINE_ENUM_OPS(typeName)                                                                                                                    \
    inline typeName operator|(typeName a, typeName b) {                                                                                              \
        return static_cast<typeName>(static_cast<u32>(a) | static_cast<u32>(b));                                                                     \
    }                                                                                                                                                \
    inline typeName& operator|=(typeName& a, typeName b) {                                                                                           \
        return a = a | b;                                                                                                                            \
    }                                                                                                                                                \
    inline typeName operator&(typeName a, typeName b) {                                                                                              \
        return static_cast<typeName>(static_cast<u32>(a) & static_cast<u32>(b));                                                                     \
    }                                                                                                                                                \
    inline typeName operator&=(typeName a, typeName b) {                                                                                             \
        return a = a & b;                                                                                                                            \
    }                                                                                                                                                \
    inline bool operator>(typeName a, typeName b) {                                                                                                  \
        return static_cast<u32>(a) > static_cast<u32>(b);                                                                                            \
    }                                                                                                                                                \
    inline bool operator>(typeName a, u32 b) {                                                                                                       \
        return static_cast<u32>(a) > b;                                                                                                              \
    }                                                                                                                                                \
    inline bool operator<(typeName a, typeName b) {                                                                                                  \
        return static_cast<u32>(a) < static_cast<u32>(b);                                                                                            \
    }                                                                                                                                                \
    inline bool operator<(typeName a, u32 b) {                                                                                                       \
        return static_cast<u32>(a) < b;                                                                                                              \
    }                                                                                                                                                \
    inline bool operator==(typeName a, u32 b) {                                                                                                      \
        return static_cast<u32>(a) == b;                                                                                                             \
    }                                                                                                                                                \
    \