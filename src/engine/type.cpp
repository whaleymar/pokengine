#include "type.h"

namespace engine {

bool isType(Type fullType, Type type) {
    return (fullType & type) > 0;
}

f32 getEffectiveness(Type source, Type target) {
    if (source == 0) {
        return Effectiveness::NEUTRAL;
    }
    return Effectiveness::NEUTRAL;  // TODO stubbed
}

}  // namespace engine