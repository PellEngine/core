#ifndef _PELLENGINE_GRAPHICS_FRAMEBUFFER_H_
#define _PELLENGINE_GRAPHICS_FRAMEBUFFER_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <pellengine/graphics/window.h>
#include <pellengine/graphics/renderpass.h>
#include <vector>

namespace pellengine {

class FrameBuffer {
 public:
  FrameBuffer(Window& window, RenderPass& renderPass);
  ~FrameBuffer();

  FrameBuffer(const FrameBuffer&) = delete;
  FrameBuffer &operator=(const FrameBuffer&) = delete;

  void initialize();
  void terminate();

 private:
  Window& window;
  RenderPass& renderPass;
  std::vector<VkFramebuffer> swapChainFrameBuffers;

};

}

#endif