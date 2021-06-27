#include "buffer.h"

namespace pellengine {

uint32_t findMemoryType(std::shared_ptr<Window> window, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(window->getInstance()->getPhysicalDevice(), &memProperties);

  for(uint32_t i=0;i<memProperties.memoryTypeCount;i++) {
    if(typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  throw std::runtime_error("Failed to find suitable memory type!");
}

bool createBuffer(std::shared_ptr<Window> window, Buffer* buffer, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags memoryProperties) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = sharingMode;

  if(vkCreateBuffer(window->getInstance()->getDevice(), &bufferInfo, nullptr, &buffer->buffer) != VK_SUCCESS) {
    return false;
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(window->getInstance()->getDevice(), buffer->buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = findMemoryType(window, memRequirements.memoryTypeBits, memoryProperties);

  if(vkAllocateMemory(window->getInstance()->getDevice(), &allocInfo, nullptr, &buffer->bufferMemory) != VK_SUCCESS) {
    return false;
  }

  vkBindBufferMemory(window->getInstance()->getDevice(), buffer->buffer, buffer->bufferMemory, 0);

  return true;
}

void terminateBuffer(std::shared_ptr<Window> window, Buffer* buffer) {
  vkDestroyBuffer(window->getInstance()->getDevice(), buffer->buffer, nullptr);
  vkFreeMemory(window->getInstance()->getDevice(), buffer->bufferMemory, nullptr);
}

}