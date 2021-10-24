#ifndef _PELLENGINE_GRAPHICS_UNIFORM_BUFFER_H_
#define _PELLENGINE_GRAPHICS_UNIFORM_BUFFER_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/buffer.h"
#include <vector>
#include <memory>

namespace pellengine {

class UniformBuffer {
 public:
  UniformBuffer(std::shared_ptr<Window> window, uint64_t bufferSize);
  ~UniformBuffer();

  UniformBuffer(const UniformBuffer&) = delete;
  UniformBuffer& operator=(const UniformBuffer&) = delete;

  void initialize();
  void terminate();

  void* map(uint32_t imageIndex);
  void unmap(uint32_t imageIndex, void* data);

  VkDescriptorBufferInfo getDescriptorBufferInfo(uint32_t imageIndex);

 private:
  bool initialized = false;
  std::shared_ptr<Window> window;
  std::vector<Buffer> buffers;
  uint64_t bufferSize;
};

}

#endif