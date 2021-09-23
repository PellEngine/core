#include "renderer.h"

namespace pellengine {

Renderer::Renderer(std::shared_ptr<Window> window) : window(window) {
  clearCommandBuffer = std::make_shared<ClearCommandBuffer>(window);
}

Renderer::~Renderer() {
  terminate();
}

void Renderer::initialize() {
  if(initialized) return;

  clearCommandBuffer->initialize();
  clearCommandBuffer->recordAll();

  imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
  imagesInFlight.resize(window->getSwapChainImages().size(), VK_NULL_HANDLE);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for(size_t i=0;i<MAX_FRAMES_IN_FLIGHT;i++) {
    if(
      vkCreateSemaphore(window->getInstance()->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS || 
      vkCreateSemaphore(window->getInstance()->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
      vkCreateFence(window->getInstance()->getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS
    ) {
      throw std::runtime_error("failed to create semaphores!");
    }
  }
  
  initialized = true;
}

void Renderer::terminate() {
  if(!initialized) return;

  clearCommandBuffer->terminate();

  for(size_t i=0;i<MAX_FRAMES_IN_FLIGHT;i++) {
    vkDestroySemaphore(window->getInstance()->getDevice(), imageAvailableSemaphores[i], nullptr);
    vkDestroySemaphore(window->getInstance()->getDevice(), renderFinishedSemaphores[i], nullptr);
    vkDestroyFence(window->getInstance()->getDevice(), inFlightFences[i], nullptr);
  }

  initialized = false;
}

uint32_t Renderer::begin() {
  vkWaitForFences(window->getInstance()->getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(window->getInstance()->getDevice(), window->getSwapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

  if(result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return imageIndex;
  } else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("Failed to acquire swap chain image.");
  }

  if(imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
    vkWaitForFences(window->getInstance()->getDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
  }

  imagesInFlight[imageIndex] = inFlightFences[currentFrame];

  return imageIndex;
}

void Renderer::end(uint32_t imageIndex, std::vector<std::shared_ptr<CommandBuffer>>& commandBuffers, bool windowResized) {
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  std::vector<VkCommandBuffer> commandBuffersFrame;
  commandBuffersFrame.push_back(clearCommandBuffer->getCommandBuffers()[imageIndex]);

  for(std::shared_ptr<CommandBuffer> commandBuffer : commandBuffers) {
    commandBuffersFrame.push_back(commandBuffer->getCommandBuffers()[imageIndex]);
  }
  submitInfo.pCommandBuffers = commandBuffersFrame.data();
  submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffersFrame.size());

  VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  vkResetFences(window->getInstance()->getDevice(), 1, &inFlightFences[currentFrame]);

  if(vkQueueSubmit(window->getInstance()->getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("Failed to submit draw command buffer");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {window->getSwapchain()};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

  VkResult result = vkQueuePresentKHR(window->getInstance()->getPresentQueue(), &presentInfo);
  if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windowResized) {
    recreateSwapChain();
  } else if(result != VK_SUCCESS) {
    throw std::runtime_error("Failed to present swap chain image.");
  }

  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::recreateSwapChain() {
  vkDeviceWaitIdle(window->getInstance()->getDevice());
  clearCommandBuffer->terminate();
  SwapChainRecreator::recreate();
  clearCommandBuffer->initialize();
  clearCommandBuffer->recordAll();
}

}