#include "renderer2D_command_buffer.h"

namespace pellengine {

Renderer2DCommandBuffer::Renderer2DCommandBuffer(std::shared_ptr<Window> window, Renderer2D* renderer2D) : CommandBuffer(window), renderer2D(renderer2D) {}
Renderer2DCommandBuffer::~Renderer2DCommandBuffer() {
  terminate();
}

void Renderer2DCommandBuffer::draw(VkCommandBuffer& commandBuffer, uint32_t imageIndex) {
  beginRenderPass(imageIndex);

  for(std::shared_ptr<Renderable2D>& renderable : renderer2D->getRenderables()) {
    renderable->render(commandBuffer, imageIndex);
  }

  endRenderPass(imageIndex);
}

}