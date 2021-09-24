#include "uniform_buffer.h"

namespace pellengine {

UniformBuffer::UniformBuffer(int binding, std::shared_ptr<Window> window, uint64_t bufferSize) : DescriptorSetProvider(binding), window(window), bufferSize(bufferSize) {}
UniformBuffer::~UniformBuffer() {
  terminate();
}

void UniformBuffer::initialize() {
  if(initialized) return;
  VkDeviceSize bufferSize = this->bufferSize;
  uniformBuffers.resize(window->getSwapChainImages().size());

  for(size_t i=0;i<window->getSwapChainImages().size();i++) {
    createBuffer(
      window,
      &uniformBuffers[i],
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
    terminateBuffer(window, &this->uniformBuffers[i]);
  }
  initialized = false;
}

void UniformBuffer::createDescriptorSetLayoutBinding() {
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = binding;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers = nullptr;
  descriptorSetLayoutBinding = uboLayoutBinding;
}

VkWriteDescriptorSet* UniformBuffer::createDescriptorWrite(uint32_t index) {
  VkDescriptorBufferInfo* bufferInfo = new VkDescriptorBufferInfo{};
  bufferInfo->buffer = uniformBuffers[index].buffer;
  bufferInfo->offset = 0;
  bufferInfo->range = bufferSize;

  VkWriteDescriptorSet* descriptorWrite = new VkWriteDescriptorSet{};
  descriptorWrite->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrite->dstBinding = binding;
  descriptorWrite->dstArrayElement = 0;
  descriptorWrite->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWrite->descriptorCount = 1;
  descriptorWrite->pBufferInfo = bufferInfo;
  descriptorWrite->pImageInfo = nullptr;
  descriptorWrite->pTexelBufferView = nullptr;

  return descriptorWrite;
}

void UniformBuffer::freeDescriptorWrite(VkWriteDescriptorSet* descriptorWrite) {
  delete descriptorWrite->pBufferInfo;
  delete descriptorWrite;
}

VkDescriptorType UniformBuffer::getDescriptorType() {
  return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
}

void* UniformBuffer::map(uint32_t imageIndex) {
  void* data;
  vkMapMemory(window->getInstance()->getDevice(), uniformBuffers[imageIndex].bufferMemory, 0 , bufferSize, 0, &data);
  return data;
}

void UniformBuffer::unmap(uint32_t imageIndex, void* data) {
  vkUnmapMemory(window->getInstance()->getDevice(), uniformBuffers[imageIndex].bufferMemory);
}

}
