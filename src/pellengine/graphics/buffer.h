#ifndef _PELLENGINE_GRAPHICS_VERTEX_BUFFER_H_
#define _PELLENGINE_GRAPHICS_VERTEX_BUFFER_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include <memory>
#include "window.h"
#include <ctti/type_id.hpp>

namespace pellengine {

struct Buffer {
  VkBuffer buffer;
  VkDeviceMemory bufferMemory;
};

bool createBuffer(std::shared_ptr<Window> window, Buffer* buffer, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags memoryProperties);
void terminateBuffer(std::shared_ptr<Window> window, Buffer* buffer);

}

#endif
