#ifndef _PELLENGINE_GRAPHICS_TEXTURE_H_
#define _PELLENGINE_GRAPHICS_TEXTURE_H_

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/image.h"
#include "src/pellengine/io/asset_reader.h"
#include <string>
#include <stb/stb_image.h>

namespace pellengine {

class Texture {
 public:
  Texture(std::shared_ptr<Window> window, std::string fileName);
  ~Texture();

  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

  void initialize();
  void terminate();

 private:
  bool initialized = false;
  std::shared_ptr<Window> window;
  std::string fileName;
  Image* textureImage;
};

}

#endif