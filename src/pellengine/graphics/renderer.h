#ifndef _PELLENGINE_GRAPHICS_RENDERER_H_
#define _PELLENGINE_GRAPHICS_RENDERER_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <pellengine/graphics/window.h>
#include <pellengine/graphics/command_buffer.h>
#include <pellengine/graphics/swapchain_recreator.h>
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
  void end(uint32_t imageIndex, CommandBuffer* commandBuffer, size_t commandBuffersSize, bool windowResized);

 private:
  size_t currentFrame = 0;
  std::shared_ptr<Window> window;
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;
};

}

#endif