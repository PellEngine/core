#ifndef _PELLENGINE_GRAPHICS_RENDERER_H_
#define _PELLENGINE_GRAPHICS_RENDERER_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/command_buffer.h"
#include "src/pellengine/graphics/swapchain_recreator.h"
#include <memory>

namespace pellengine {

class Renderer {
 public:
  Renderer(std::shared_ptr<Window> window);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  void initialize();
  void terminate();
  uint32_t begin();
  void end(uint32_t imageIndex, std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers, bool windowResized);

 private:
  bool initialized = false;
  size_t currentFrame = 0;
  std::shared_ptr<Window> window;
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;
};

}

#endif