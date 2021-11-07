#ifndef _PELLENGINE_GRAPHICS_SPRITE_SHEET_H_
#define _PELLENGINE_GRAPHICS_SPRITE_SHEET_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/texture.h"
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <array>
#include <math.h>

namespace pellengine {

class SpriteSheet {
 public:
  SpriteSheet(std::shared_ptr<Window> window, std::string fileName, int width, int height, int spacing);
  ~SpriteSheet();

  SpriteSheet& operator=(const SpriteSheet&) = delete;

  void initialize();
  void terminate();

  std::array<glm::vec2, 4>& getTexCoords(int x, int y);
  std::shared_ptr<Texture> getTexture();

 private:
  bool initialized = false;
  std::string fileName;
  int width;
  int height;
  int spacing;
  std::shared_ptr<Texture> texture;
  std::vector<std::array<glm::vec2, 4>> texCoords;
};

}

#endif