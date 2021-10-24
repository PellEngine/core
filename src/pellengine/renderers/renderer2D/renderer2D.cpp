#include "renderer2D.h"

namespace pellengine {

Renderer2D::Renderer2D(std::shared_ptr<Window> window) : window(window) {
  commandBuffer = std::make_shared<Renderer2DCommandBuffer>(window, this);
  SwapChainRecreator::registerCommandBuffer(commandBuffer);
}
Renderer2D::~Renderer2D() {
  terminate();
}

void Renderer2D::initialize() {
  commandBuffer->initialize();
}

void Renderer2D::terminate() {
  commandBuffer->terminate();
}

void Renderer2D::render(uint32_t imageIndex) {
  commandBuffer->record(imageIndex);
}

}