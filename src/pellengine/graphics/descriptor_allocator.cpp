#include "descriptor_allocator.h"

namespace pellengine {

DescriptorAllocator::DescriptorAllocator(std::shared_ptr<Window> window) : window(window) {}
DescriptorAllocator::~DescriptorAllocator() {
  terminate();
}

void DescriptorAllocator::terminate() {
  for(auto pool : freePools) {
    vkDestroyDescriptorPool(window->getInstance()->getDevice(), pool, nullptr);
  }
  for(auto pool : usedPools) {
    vkDestroyDescriptorPool(window->getInstance()->getDevice(), pool, nullptr);
  }
}

VkDescriptorPool DescriptorAllocator::createPool(PoolSizes& poolSizes, int numSets, VkDescriptorPoolCreateFlags flags) {
  std::vector<VkDescriptorPoolSize> sizes;
  sizes.reserve(poolSizes.sizes.size());
  for(std::pair<VkDescriptorType, float> size : poolSizes.sizes) {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = size.first;
    poolSize.descriptorCount = uint32_t(size.second * numSets * window->getSwapChainImages().size());
    sizes.push_back(poolSize);
  }

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.flags = flags;
  poolInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
  poolInfo.pPoolSizes = sizes.data();
  poolInfo.maxSets = window->getSwapChainImages().size() * numSets;

  VkDescriptorPool descriptorPool;
  if(vkCreateDescriptorPool(window->getInstance()->getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor pool.");
  }

  return descriptorPool;
}

VkDescriptorPool DescriptorAllocator::getPool() {
  if(freePools.size() > 0) {
    VkDescriptorPool pool = freePools.back();
    freePools.pop_back();
    return pool;
  } else {
    return createPool(descriptorSizes, 1000, 0);
  }
}

bool DescriptorAllocator::allocate(VkDescriptorSet* descriptorSet, VkDescriptorSetLayout layout) {
  if(currentPool == VK_NULL_HANDLE) {
    currentPool = getPool();
    usedPools.push_back(currentPool);
  }

  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = currentPool;
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts = &layout;

  VkResult allocResult = vkAllocateDescriptorSets(window->getInstance()->getDevice(), &allocInfo, descriptorSet);
  bool reallocate = false;

  switch(allocResult) {
    case VK_SUCCESS:
      return true;
    case VK_ERROR_FRAGMENTED_POOL:
    case VK_ERROR_OUT_OF_POOL_MEMORY:
      reallocate = true;
      break;
    default:
      return false;
  }

  if(reallocate) {
    currentPool = getPool();
    usedPools.push_back(currentPool);
    allocInfo.descriptorPool = currentPool;

    allocResult = vkAllocateDescriptorSets(window->getInstance()->getDevice(), &allocInfo, descriptorSet);
    if(allocResult == VK_SUCCESS) {
      return true;
    }
  }

  return false;
}

void DescriptorAllocator::resetPools() {
  for(VkDescriptorPool& pool : usedPools) {
    vkResetDescriptorPool(window->getInstance()->getDevice(), pool, 0);
    freePools.push_back(pool);
  }
  usedPools.clear();
  currentPool = VK_NULL_HANDLE;
}

}