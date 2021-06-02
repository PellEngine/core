#ifndef _PELLENGINE_COMPONENTS_TRANSFORM_H_
#define _PELLENGINE_COMPONENTS_TRANSFORM_H_

#include <glm/vec2.hpp>

namespace pellengine {

struct Transform {
  glm::vec2 position;
  glm::vec2 scale;
};

}

#endif