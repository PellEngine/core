#ifndef _PELLENGINE_GRAPHICS_UNIFORM_BUFFER_H_
#define _PELLENGINE_GRAPHICS_UNIFORM_BUFFER_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/buffer.h"
#include <vector>
#include <memory>

namespace pellengine {

class UniformBuffer {
 public:
  UniformBuffer(std::shared_ptr<Window> window, int binding);
  virtual ~UniformBuffer();

  UniformBuffer(const UniformBuffer&) = delete;
  UniformBuffer& operator=(const UniformBuffer&) = delete;

  void initialize();
  void recreateSwapChain();
  void terminate();
  void terminateSwapChain();
  
  std::vector<VkDescriptorSetLayout>& getDescriptorSetLayouts() {
    return descriptorSetLayouts;
  }

  std::vector<Buffer>& getBuffers() {
    return uniformBuffers;
  }

  int getBinding() {
    return binding;
  }

  virtual size_t getUniformSize() = 0;

 private:
  bool initialized = false;
  int binding;

 protected:
  std::shared_ptr<Window> window;
  std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
  std::vector<Buffer> uniformBuffers;

  virtual void createDescriptorSetLayouts() = 0;
  virtual void createBuffers() = 0;
};

}

#endif