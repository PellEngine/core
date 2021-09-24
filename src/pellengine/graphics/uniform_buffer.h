#ifndef _PELLENGINE_GRAPHICS_UNIFORM_BUFFER_H_
#define _PELLENGINE_GRAPHICS_UNIFORM_BUFFER_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/buffer.h"
#include "src/pellengine/graphics/descriptor_set_provider.h"
#include <vector>
#include <memory>

namespace pellengine {

class UniformBuffer : public DescriptorSetProvider {
 public:
  UniformBuffer(int binding, std::shared_ptr<Window> window, uint64_t bufferSize);
  virtual ~UniformBuffer();

  UniformBuffer(const UniformBuffer&) = delete;
  UniformBuffer& operator=(const UniformBuffer&) = delete;

  void createDescriptorSetLayoutBinding() override;
  VkWriteDescriptorSet* createDescriptorWrite(uint32_t index) override;
  VkDescriptorType getDescriptorType() override;
  void freeDescriptorWrite(VkWriteDescriptorSet* descriptorWrite) override;

  void initialize();
  void terminate();

  void* map(uint32_t imageIndex);
  void unmap(uint32_t imageIndex, void* data);

 private:
  std::shared_ptr<Window> window;
  uint64_t bufferSize;
  bool initialized = false;
  std::vector<Buffer> uniformBuffers;
};

}

#endif