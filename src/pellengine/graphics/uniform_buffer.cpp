#include "uniform_buffer.h"

namespace pellengine {

UniformBuffer::UniformBuffer(std::shared_ptr<Window> window, uint64_t bufferSize) : window(window), bufferSize(bufferSize) {}
UniformBuffer::~UniformBuffer() {
  terminate();
}

void UniformBuffer::initialize() {
  if(initialized) return;
  VkDeviceSize bufferSize = this->bufferSize;
  buffers.resize(window->getSwapChainImages().size());

  for(size_t i=0;i<window->getSwapChainImages().size();i++) {
    createBuffer(
      window,
      &buffers[i],
      bufferSize,
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      VK_SHARING_MODE_EXCLUSIVE,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
  }

  initialized = true;
}

void UniformBuffer::terminate() {
  if(!initialized) return;
  for(size_t i=0;i<window->getSwapChainImages().size();i++) {
    terminateBuffer(window, &this->buffers[i]);
  }
  initialized = false;
}

void* UniformBuffer::map(uint32_t imageIndex) {
  void* data;
  vkMapMemory(window->getInstance()->getDevice(), buffers[imageIndex].bufferMemory, 0 , bufferSize, 0, &data);
  return data;
}

void UniformBuffer::unmap(uint32_t imageIndex, void* data) {
  vkUnmapMemory(window->getInstance()->getDevice(), buffers[imageIndex].bufferMemory);
}

VkDescriptorBufferInfo UniformBuffer::getDescriptorBufferInfo(uint32_t imageIndex) {
  VkDescriptorBufferInfo bufferInfo{};
  bufferInfo.buffer = buffers[imageIndex].buffer;
  bufferInfo.offset = 0;
  bufferInfo.range = bufferSize;
  return bufferInfo;
}

}
