#ifndef _PELLENGINE_RENDERERS_RENDERER2D_RENDERER2D_H_
#define _PELLENGINE_RENDERERS_RENDERER2D_RENDERER2D_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/swapchain_recreator.h"
#include "src/pellengine/graphics/renderer.h"
#include "renderer2D_command_buffer.h"

namespace pellengine {

// Forward declaration
class Renderer2DCommandBuffer;

class Renderable2D {
 public:
  virtual void render(VkCommandBuffer& commandBuffer, uint32_t imageIndex) = 0;
};

class Renderer2D {
 public:
  Renderer2D(std::shared_ptr<Window> window);
  ~Renderer2D();

  Renderer2D(const Renderer2D&) = delete;
  Renderer2D& operator=(const Renderer2D&) = delete;

  void initialize();
  void terminate();

  std::vector<std::shared_ptr<Renderable2D>>& getRenderables() {
    return renderables;
  }

  void addRenderable(std::shared_ptr<Renderable2D> renderable) {
    renderables.push_back(renderable);
  }

  void removeRenderable(std::shared_ptr<Renderable2D> renderable) {
    renderables.erase(std::remove(renderables.begin(), renderables.end(), renderable));
  }

  void render(uint32_t imageIndex);
  
  std::shared_ptr<CommandBuffer> getCommandBuffer() {
    return std::static_pointer_cast<CommandBuffer>(commandBuffer);
  }

 private:
  std::shared_ptr<Window> window;
  std::shared_ptr<Renderer2DCommandBuffer> commandBuffer;
  std::vector<std::shared_ptr<Renderable2D>> renderables;
};

}

#endif