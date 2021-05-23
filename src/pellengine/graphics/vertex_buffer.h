#ifndef _PELLENGINE_GRAPHICS_VERTEX_BUFFER_H_
#define _PELLENGINE_GRAPHICS_VERTEX_BUFFER_H_

#include <pellengine/graphics/window.h>
#include <pellengine/vulkan/vulkan_wrapper.h>
#include <memory>

namespace pellengine {

struct VertexBuffer {
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
};

bool createVertexBuffer(std::shared_ptr<Window> window, VertexBuffer* buffer, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags memoryProperties);
void terminateVertexBuffer(std::shared_ptr<Window> window, VertexBuffer* buffer);

}

#endif