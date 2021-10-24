#ifndef _PELLENGINE_RENDERERS_RENDERER2D_COMMAND_BUFFER_H_
#define _PELLENGINE_RENDERERS_RENDERER2D_COMMAND_BUFFER_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/command_buffer.h"
#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include "renderer2D.h"

namespace pellengine {

// Forward declaration
class Renderer2D;

class Renderer2DCommandBuffer : public CommandBuffer {
 public:
  Renderer2DCommandBuffer(std::shared_ptr<Window> window, Renderer2D* renderer2D);
  ~Renderer2DCommandBuffer();

  Renderer2DCommandBuffer(const Renderer2DCommandBuffer&) = delete;
  Renderer2DCommandBuffer& operator=(const Renderer2DCommandBuffer&) = delete;

 protected:
  void draw(VkCommandBuffer& commandBuffer, uint32_t imageIndex) override;

 private:
  Renderer2D* renderer2D;
};

}

#endif