#include "descriptor_builder.h"

namespace pellengine {

DescriptorBuilder DescriptorBuilder::begin(std::shared_ptr<Window> window, std::shared_ptr<DescriptorAllocator> allocator) {
  DescriptorBuilder builder;
  builder.window = window;
  builder.allocator = allocator;
  builder.writes.resize(window->getSwapChainImages().size());
  return builder;
}

DescriptorBuilder& DescriptorBuilder::bindBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags) {
  VkDescriptorSetLayoutBinding layoutBinding{};
  layoutBinding.binding = binding;
  layoutBinding.descriptorType = type;
  layoutBinding.descriptorCount = 1;
  layoutBinding.stageFlags = stageFlags;
  layoutBinding.pImmutableSamplers = nullptr;
  bindings.push_back(layoutBinding);

  for(size_t i=0;i<window->getSwapChainImages().size();i++) {
    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.descriptorType = type;
    write.descriptorCount = 1;
    write.pBufferInfo = &bufferInfo[i];
    write.pImageInfo = nullptr;
    write.pTexelBufferView = nullptr;
    writes[i].push_back(write);
  }

  return *this;
}

DescriptorBuilder& DescriptorBuilder::bindTexture(uint32_t binding, VkDescriptorImageInfo* imageInfo, uint32_t imageInfoCount, VkDescriptorType type, VkShaderStageFlags stageFlags) {
  VkDescriptorSetLayoutBinding layoutBinding{};
  layoutBinding.binding = binding;
  layoutBinding.descriptorType = type;
  layoutBinding.descriptorCount = imageInfoCount;
  layoutBinding.stageFlags = stageFlags;
  layoutBinding.pImmutableSamplers = nullptr;
  bindings.push_back(layoutBinding);

  for(size_t i=0;i<window->getSwapChainImages().size();i++) {
    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.descriptorType = type;
    write.descriptorCount = imageInfoCount;
    write.pImageInfo = imageInfo;
    writes[i].push_back(write);
  }

  return *this;
}

bool DescriptorBuilder::build(uint32_t descriptorSetCount, VkDescriptorSet* descriptorSet, VkDescriptorSetLayout& layout) {
  // Create layout
  VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
  layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
  layoutCreateInfo.pBindings = bindings.data();

  if(vkCreateDescriptorSetLayout(window->getInstance()->getDevice(), &layoutCreateInfo, nullptr, &layout) != VK_SUCCESS) {
    return false;
  }

  // Allocate descriptor set
  for(uint32_t i=0;i<descriptorSetCount;i++) {
    bool success = allocator->allocate(&descriptorSet[i], layout);
    if(!success) return false;

    for(VkWriteDescriptorSet& write : writes[i]) {
      write.dstSet = descriptorSet[i];
    }

    vkUpdateDescriptorSets(window->getInstance()->getDevice(), writes[i].size(), writes[i].data(), 0, nullptr);
  }

  return true;
}

}