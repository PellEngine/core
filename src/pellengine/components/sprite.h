#ifndef _PELLENGINE_COMPONENTS_SPRITE_H_
#define _PELLENGINE_COMPONENTS_SPRITE_H_

#include <glm/vec4.hpp>

namespace pellengine {

struct Sprite {
  glm::vec4 color;
  int zIndex;
  bool dirty;
};

}

#endif