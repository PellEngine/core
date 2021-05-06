#ifndef _PELLENGINE_GRAPHICS_RENDERPASS_H_
#define _PELLENGINE_GRAPHICS_RENDERPASS_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <stdexcept>
#include <pellengine/graphics/window.h>

namespace pellengine {

class RenderPass {
 public:
  RenderPass(Window& window);
  ~RenderPass();

  RenderPass(const RenderPass&) = delete;
  RenderPass &operator=(const RenderPass&) = delete;

  void initialize();
  void terminate();

  VkRenderPass getVkRenderPass() {
    return renderPass;
  }

 private:
  Window& window;
  VkRenderPass renderPass;
};

}

#endif