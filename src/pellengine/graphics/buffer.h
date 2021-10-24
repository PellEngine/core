#ifndef _PELLENGINE_GRAPHICS_VERTEX_BUFFER_H_
#define _PELLENGINE_GRAPHICS_VERTEX_BUFFER_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include <memory>
#include "window.h"

namespace pellengine {

struct Buffer {
  VkBuffer buffer;
  VkDeviceMemory bufferMemory;

  void* map(std::shared_ptr<Window> window, VkDeviceSize offset, VkDeviceSize size) {
    void* data;
    vkMapMemory(window->getInstance()->getDevice(), bufferMemory, offset, size, 0, &data);
    return data;
  }

  void unmap(std::shared_ptr<Window> window) {
    vkUnmapMemory(window->getInstance()->getDevice(), bufferMemory);
  }
};

uint32_t findMemoryType(std::shared_ptr<Window> window, uint32_t typeFilter, VkMemoryPropertyFlags properties);
bool createBuffer(std::shared_ptr<Window> window, Buffer* buffer, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags memoryProperties);
void terminateBuffer(std::shared_ptr<Window> window, Buffer* buffer);

}

#endif
