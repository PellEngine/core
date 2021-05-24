#ifndef _PELLENGINE_ECS_COMPONENT_H_
#define _PELLENGINE_ECS_COMPONENT_H_

#include <bitset>

namespace pellengine {

using ComponentType = uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

}

#endif