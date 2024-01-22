#include "type.h"

namespace engine {

bool isType(Type fullType, Type type) {
    return (fullType & type) > 0;
}

}  // namespace engine