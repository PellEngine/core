#ifndef _PELLENGINE_GRAPHICS_TEXTURE_EMPTY_H_
#define _PELLENGINE_GRAPHICS_TEXTURE_EMPTY_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/image.h"

namespace pellengine {

class TextureEmpty {
 public:
  TextureEmpty(std::shared_ptr<Window> window);
  ~TextureEmpty();

  TextureEmpty(const TextureEmpty&) = delete;
  TextureEmpty& operator=(const TextureEmpty&) = delete;

  void initialize();
  void terminate();

  VkImageView getImageView() {
    return textureImageView;
  }

  VkSampler getSampler() {
    return textureSampler;
  }

 private:
  bool initialized = false;
  std::shared_ptr<Window> window;
  Image* textureImage;
  VkImageView textureImageView;
  VkSampler textureSampler;
};

}

#endif