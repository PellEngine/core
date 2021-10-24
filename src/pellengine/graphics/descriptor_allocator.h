#ifndef _PELLENGINE_GRAPHICS_DESCRIPTOR_ALLOCATOR_H_
#define _PELLENGINE_GRAPHICS_DESCRIPTOR_ALLOCATOR_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include <vector>

namespace pellengine {

class DescriptorAllocator {
 public:
  DescriptorAllocator(std::shared_ptr<Window> window);
  ~DescriptorAllocator();

  struct PoolSizes {
    std::vector<std::pair<VkDescriptorType, float>> sizes = {
      { VK_DESCRIPTOR_TYPE_SAMPLER, 0.5f },
      { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.f },
      { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 4.f },
      { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1.f },
      { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1.f },
      { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1.f },
      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2.f },
      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2.f },
      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f },
      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f },
      { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0.5f }
    };
  };

  DescriptorAllocator(const DescriptorAllocator&) = delete;
  DescriptorAllocator& operator=(const DescriptorAllocator&) = delete;

  void resetPools();
  bool allocate(VkDescriptorSet* descriptorSet, VkDescriptorSetLayout layout);
  void terminate();

 private:
  std::shared_ptr<Window> window;
  PoolSizes descriptorSizes;
  std::vector<VkDescriptorPool> usedPools;
  std::vector<VkDescriptorPool> freePools;
  VkDescriptorPool currentPool{VK_NULL_HANDLE};

  VkDescriptorPool getPool();
  VkDescriptorPool createPool(PoolSizes& poolSizes, int numSets, VkDescriptorPoolCreateFlags flags);
};

}

#endif