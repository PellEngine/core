#ifndef _PELLENGINE_GRAPHICS_COMMAND_BUFFER_H_
#define _PELLENGINE_GRAPHICS_COMMAND_BUFFER_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include <vector>

namespace pellengine {

class CommandBuffer {
 public:
  CommandBuffer(std::shared_ptr<Window> window);
  virtual ~CommandBuffer();

  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer& operator=(const CommandBuffer&) = delete;

  void initialize();
  void terminate();
  void recordAll();
  void record(uint32_t imageIndex);

  std::vector<VkCommandBuffer>& getCommandBuffers() {
    return commandBuffers;
  }

 protected:
  std::shared_ptr<Window> window;
  std::vector<VkCommandBuffer> commandBuffers;

  virtual void draw(VkCommandBuffer& buffer, uint32_t imageIndex) = 0;
  
  void beginRenderPass(uint32_t i);
  void endRenderPass(uint32_t i);
  void bindPipeline(uint32_t i, VkPipelineBindPoint bindPoint, VkPipeline pipeline);

 private:
  bool initialized = false;
};

}

#endif