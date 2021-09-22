#include "uniform_buffer.h"

namespace pellengine {

UniformBuffer::UniformBuffer(std::shared_ptr<Window> window, int binding) : binding(binding), window(window) {}
UniformBuffer::~UniformBuffer() {
  terminate();
}

void UniformBuffer::initialize() {
  if(initialized) return;
  createDescriptorSetLayouts();
  createBuffers();
  initialized = true;
}

void UniformBuffer::recreateSwapChain() {
  createBuffers();
}

void UniformBuffer::terminateSwapChain() {
  for(size_t i=0;i<window->getSwapChainImages().size();i++) {
    terminateBuffer(window, &this->uniformBuffers[i]);
  }
}

void UniformBuffer::terminate() {
  if(!initialized) return;
  terminateSwapChain();
  for(auto descriptorSetLayout : descriptorSetLayouts) {
    vkDestroyDescriptorSetLayout(window->getInstance()->getDevice(), descriptorSetLayout, nullptr);
  }
  initialized = false;
}

}
