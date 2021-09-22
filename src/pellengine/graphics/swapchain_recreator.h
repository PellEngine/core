#ifndef _PELLENGINE_GRAPHICS_SWAPCHAIN_RECREATOR_H_
#define _PELLENGINE_GRAPHICS_SWAPCHAIN_RECREATOR_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/command_buffer.h"
#include "src/pellengine/graphics/graphics_pipeline.h"
#include "src/pellengine/graphics/uniform_buffer.h"
#include "src/pellengine/io/logger.h"
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
  static void registerUniformBuffer(std::shared_ptr<UniformBuffer> uniformBuffer);
  static void recreate();

 private:
  std::weak_ptr<Window> windowPtr;
  std::vector<std::weak_ptr<CommandBuffer>> commandBuffers;
  std::vector<std::weak_ptr<GraphicsPipeline>> graphicsPipelines;
  std::vector<std::weak_ptr<UniformBuffer>> uniformBuffers;

 protected:
  SwapChainRecreator();
  static SwapChainRecreator* singleton_;
};

}

#endif