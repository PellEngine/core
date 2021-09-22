#include "sprite_batch_uniform_buffer.h"

namespace pellengine {

SpriteBatchUniformBuffer::SpriteBatchUniformBuffer(std::shared_ptr<Window> window, int binding) : UniformBuffer(window, binding) {}
SpriteBatchUniformBuffer::~SpriteBatchUniformBuffer() {}

void SpriteBatchUniformBuffer::createDescriptorSetLayouts() {
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers = nullptr;

  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.pBindings = &uboLayoutBinding;
  layoutInfo.bindingCount = 1;

  descriptorSetLayouts.resize(1);
  if(vkCreateDescriptorSetLayout(window->getInstance()->getDevice(), &layoutInfo, nullptr, descriptorSetLayouts.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout.");
  }
}

void SpriteBatchUniformBuffer::createBuffers() {
  VkDeviceSize bufferSize = sizeof(SpriteBatchUniformBufferObject);
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
}

void SpriteBatchUniformBuffer::updateUniformBuffer(uint32_t imageIndex) {
  SpriteBatchUniformBufferObject ubo{};
  ubo.proj = glm::ortho(0.0f, (float)window->getSwapChainExtent().width, 0.0f, (float)window->getSwapChainExtent().height);
  void* data;
  vkMapMemory(window->getInstance()->getDevice(), uniformBuffers[imageIndex].bufferMemory, 0, sizeof(ubo), 0, &data);
  memcpy(data, &ubo, sizeof(ubo));
  vkUnmapMemory(window->getInstance()->getDevice(), uniformBuffers[imageIndex].bufferMemory);
}

}