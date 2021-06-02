#ifndef _PELLENGINE_ECS_TYPES_H_
#define _PELLENGINE_ECS_TYPES_H_

#include <stdint.h>
#include <bitset>

namespace pellengine {
  
const uint32_t MAX_COMPONENTS = 32;
const uint32_t MAX_ENTITIES = 4096;

typedef uint32_t Identifier;
typedef Identifier Entity;
typedef Identifier ComponentType;
using Signature = std::bitset<MAX_COMPONENTS>;
const Identifier NULL_ENTITY = 0; 

}

#endif