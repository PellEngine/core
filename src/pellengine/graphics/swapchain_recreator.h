#ifndef _PELLENGINE_GRAPHICS_SWAPCHAIN_RECREATOR_H_
#define _PELLENGINE_GRAPHICS_SWAPCHAIN_RECREATOR_H_

#include <pellengine/graphics/window.h>
#include <pellengine/graphics/command_buffer.h>
#include <pellengine/graphics/graphics_pipeline.h>
#include <pellengine/io/logger.h>
#include <vector>
#include <memory>

namespace pellengine {

class SwapChainRecreator {
 public:
  SwapChainRecreator(SwapChainRecreator&) = delete;
  SwapChainRecreator& operator=(SwapChainRecreator&) = delete;

  static SwapChainRecreator* getInstance();

  static void registerWindow(std::shared_ptr<Window> window);
  static void registerCommandBuffer(std::shared_ptr<CommandBuffer> commandBuffer);
  static void registerGraphicsPipeline(std::shared_ptr<GraphicsPipeline> graphicsPipeline);
  static void recreate();

 private:
  std::weak_ptr<Window> windowPtr;
  std::vector<std::weak_ptr<CommandBuffer>> commandBuffers;
  std::vector<std::weak_ptr<GraphicsPipeline>> graphicsPipelines;

 protected:
  SwapChainRecreator();
  static SwapChainRecreator* singleton_;
};

}

#endif