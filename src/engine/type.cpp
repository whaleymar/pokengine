#include "type.h"

namespace engine {

bool isType(Type fullType, Type type) {
    return (fullType & type) > 0;
}

f32 getEffectiveness(Type source, Type target) {
    if (source == 0) {
        return Effectiveness::NEUTRAL;
    }
    // make sure I consider multi-type sources (maybe only for testing ineffectiveness)
    return Effectiveness::NEUTRAL;  // TODO stubbed
}

}  // namespace engine
