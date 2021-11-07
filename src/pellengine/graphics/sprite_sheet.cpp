#include "sprite_sheet.h"

namespace pellengine {

SpriteSheet::SpriteSheet(std::shared_ptr<Window> window, std::string fileName, int width, int height, int spacing) : fileName(fileName), width(width), height(height), spacing(spacing) {
  texture = std::make_shared<Texture>(window, fileName);

  // Generate texture coordinates
  float spriteWidth = 1.0f / width;
  float spriteHeight = 1.0f / height;

  for(int y=0;y<height;y++) {
    for(int x=0;x<width;x++) {
      float textureX =  x * spriteWidth;
      float textureY = 1.0f - (y * spriteHeight);

      std::array<glm::vec2, 4> coord = {{
        {textureX + spriteWidth, textureY - spriteHeight},
        {textureX, textureY - spriteHeight},
        {textureX, textureY},
        {textureX + spriteWidth, textureY}
      }};

      texCoords.push_back(coord);
    }
  }
}
SpriteSheet::~SpriteSheet() {
  terminate();
}

void SpriteSheet::initialize() {
  if(initialized) return;
  texture->initialize();
  initialized = true;
}

void SpriteSheet::terminate() {
  if(!initialized) return;
  texture->terminate();
  initialized = false;
}

std::array<glm::vec2, 4>& SpriteSheet::getTexCoords(int x, int y) {
  int index = x + y * width;
  return texCoords[index];
}

std::shared_ptr<Texture> SpriteSheet::getTexture() {
  return texture;
}

}