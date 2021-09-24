#ifndef _PELLENGINE_GRAPHICS_DESCRIPTOR_SET_PROVIDER_H_
#define _PELLENGINE_GRAPHICS_DESCRIPTOR_SET_PROVIDER_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include <vector>

namespace pellengine {

class DescriptorSetProvider {
 public:
  DescriptorSetProvider(int binding) : binding(binding) {}

  virtual void createDescriptorSetLayoutBinding() = 0;
  virtual VkDescriptorType getDescriptorType() = 0;
  virtual VkWriteDescriptorSet* createDescriptorWrite(uint32_t index) = 0;
  virtual void freeDescriptorWrite(VkWriteDescriptorSet* descriptorWrite) = 0;

  VkDescriptorSetLayoutBinding getDescriptorSetLayoutBinding();

  int getBinding() {
    return binding;
  }

 protected:
  int binding;
  VkDescriptorSetLayoutBinding descriptorSetLayoutBinding;
};

}

#endif