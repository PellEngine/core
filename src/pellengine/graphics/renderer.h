#ifndef _PELLENGINE_GRAPHICS_RENDERER_H_
#define _PELLENGINE_GRAPHICS_RENDERER_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <pellengine/graphics/window.h>
#include <pellengine/graphics/command_buffer.h>

namespace pellengine {

class Renderer {
 public:
  Renderer(Window& window);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  void initialize();
  void terminate();
  void draw(Window& window, CommandBuffer& commandBuffer, size_t commandBuffersSize);

 private:
  size_t currentFrame = 0;
  Window& window;
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;
};

}

#endif