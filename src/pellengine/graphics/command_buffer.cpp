#include "command_buffer.h"

namespace pellengine {

CommandBuffer::CommandBuffer(Window& window, PipelineConfiguration pipelineConfiguration) : window(window), pipelineConfiguration(pipelineConfiguration) {}

CommandBuffer::~CommandBuffer() {}

void CommandBuffer::initialize() {
  commandBuffers.resize(window.getSwapChainImageViews().size());
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = window.getCommandPool();
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

  if(vkAllocateCommandBuffers(window.getInstance()->getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create command buffers.");
  }
}

void CommandBuffer::record() {
  for(size_t i=0;i<commandBuffers.size();i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("Failed to begin recording command buffers.");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = window.getRenderPass();
    renderPassInfo.framebuffer = window.getSwapChainFrameBuffers()[i];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = window.getSwapChainExtent();

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    switch(pipelineConfiguration.bindPoint) {
      case VK_PIPELINE_BIND_POINT_GRAPHICS: {
        vkCmdBindPipeline(commandBuffers[i], pipelineConfiguration.bindPoint, pipelineConfiguration.graphicsPipeline.value()->getPipeline());
        break;
      }
    }

    draw(commandBuffers[i]);

    vkCmdEndRenderPass(commandBuffers[i]);
    if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to record command buffer.");
    }
  }
}

}