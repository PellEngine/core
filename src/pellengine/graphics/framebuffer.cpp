#include "framebuffer.h"

namespace pellengine {

FrameBuffer::FrameBuffer(Window& window, RenderPass& renderPass) : window(window), renderPass(renderPass) {}
FrameBuffer::~FrameBuffer() {
  this->terminate();
}

void FrameBuffer::initialize() {
  std::vector<VkImageView> swapChainImageViews = window.getSwapChainImageViews();
  swapChainFrameBuffers.resize(swapChainImageViews.size());

  for(size_t i=0;i<swapChainImageViews.size();i++) {
    VkImageView attachments[] = {
      swapChainImageViews[i]
    };

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass.getVkRenderPass();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = window.getSwapChainExtent().width;
    framebufferInfo.height = window.getSwapChainExtent().height;
    framebufferInfo.layers = 1;

    if(vkCreateFramebuffer(window.getDevice(), &framebufferInfo, nullptr, &swapChainFrameBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create framebuffer");
    }
  }
}

void FrameBuffer::terminate() {
  for(auto framebuffer : swapChainFrameBuffers) {
    vkDestroyFramebuffer(window.getDevice(), framebuffer, nullptr);
  }
}

}