#ifndef _PELLENGINE_GRAPHICS_IMAGE_H_
#define _PELLENGINE_GRAPHICS_IMAGE_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/buffer.h"
#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include <string>

namespace pellengine {

struct ImageConfiguration {
  VkFormat format;
  VkImageTiling tiling;
  VkImageUsageFlags usage;
  VkMemoryPropertyFlags properties;
};

class Image {
 public:
  Image(std::shared_ptr<Window> window, uint32_t width, uint32_t height, ImageConfiguration configuration);
  ~Image();

  Image(const Image&) = delete;
  Image& operator=(const Image&) = delete;

  void initialize();
  void terminate();
  void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
  void copyBuffer(VkBuffer buffer);

  VkImage getImage() {
    return image;
  }

 private:
  bool initialized = false;
  std::shared_ptr<Window> window;
  uint32_t width;
  uint32_t height;
  ImageConfiguration configuration;
  VkImage image;
  VkDeviceMemory imageMemory;
};

}

#endif