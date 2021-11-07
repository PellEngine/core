#ifndef _PELLENGINE_COMPONENTS_SPRITE_H_
#define _PELLENGINE_COMPONENTS_SPRITE_H_

#include "src/pellengine/graphics/sprite_sheet.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>

namespace pellengine {

struct Sprite {
  std::shared_ptr<SpriteSheet> spriteSheet;
  glm::vec2 spritePos;
  glm::vec4 color;
  float zIndex;
  bool dirty;
};

}

#endif