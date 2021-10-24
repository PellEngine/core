#ifndef _PELLENGINE_GRAPHICS_DESCRIPTOR_BUILDER_H_
#define _PELLENGINE_GRAPHICS_DESCRIPTOR_BUILDER_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/descriptor_allocator.h"
#include "src/pellengine/graphics/texture.h"  
#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include <vector>

namespace pellengine {

class DescriptorBuilder {
 public:
  static DescriptorBuilder begin(std::shared_ptr<Window> window, std::shared_ptr<DescriptorAllocator> allocator);

  DescriptorBuilder& bindBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);
  DescriptorBuilder& bindTexture(uint32_t binding, VkDescriptorImageInfo* imageInfo, VkDescriptorType type, VkShaderStageFlags);

  bool build(uint32_t descriptorSetCount, VkDescriptorSet* descriptorSet, VkDescriptorSetLayout& layout);

 private:
  std::shared_ptr<Window> window;
  std::shared_ptr<DescriptorAllocator> allocator;
  std::vector<VkDescriptorSetLayoutBinding> bindings;
  std::vector<std::vector<VkWriteDescriptorSet>> writes;
};

}

#endif