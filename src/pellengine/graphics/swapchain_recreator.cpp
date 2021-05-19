#include "swapchain_recreator.h"

namespace pellengine {

SwapChainRecreator* SwapChainRecreator::singleton_ = nullptr;

SwapChainRecreator* SwapChainRecreator::getInstance() {
  if(singleton_ == nullptr) {
    singleton_ = new SwapChainRecreator();
  }

  return singleton_;
}

SwapChainRecreator::SwapChainRecreator() {}

void SwapChainRecreator::registerWindow(std::shared_ptr<Window> window) {
  getInstance()->windowPtr = window;
}

void SwapChainRecreator::registerCommandBuffer(std::shared_ptr<CommandBuffer> commandBuffer) {
  getInstance()->commandBuffers.push_back(commandBuffer);
}

void SwapChainRecreator::registerGraphicsPipeline(std::shared_ptr<GraphicsPipeline> graphicsPipeline) {
  getInstance()->graphicsPipelines.push_back(graphicsPipeline);
}

void SwapChainRecreator::recreate() {
  // Make sure that window pointer still manages an object 
  if(getInstance()->windowPtr.expired()) {
    return;
  }

  // Access window through a shared pointer
  if(auto window = getInstance()->windowPtr.lock()) {
    vkDeviceWaitIdle(window->getInstance()->getDevice());

    std::vector<std::weak_ptr<CommandBuffer>>& commandBuffers = getInstance()->commandBuffers;
    std::vector<std::weak_ptr<GraphicsPipeline>>& graphicsPipelines = getInstance()->graphicsPipelines;

    // Terminate swap chain resources
    for(auto it=commandBuffers.begin();it!=commandBuffers.end();) {
      if(auto commandBuffer = it->lock()) {
        commandBuffer->terminate();
        it++;
      } else {
        commandBuffers.erase(it);
      }
    }

    for(auto it=graphicsPipelines.begin();it!=graphicsPipelines.end();) {
      if(auto graphicsPipeline = it->lock()) {
        graphicsPipeline->terminate();
        it++;
      } else {
        graphicsPipelines.erase(it);
      }
    }

    window->terminateSwapChain();

    // Recreate swap chain resources
    window->recreateSwapChain();
    
    for(auto it=graphicsPipelines.begin();it!=graphicsPipelines.end();) {
      if(auto graphicsPipeline = it->lock()) {
        graphicsPipeline->initialize();
        it++;
      } else {
        graphicsPipelines.erase(it);
      }
    }

    for(auto it=commandBuffers.begin();it!=commandBuffers.end();) {
      if(auto commandBuffer = it->lock()) {
        commandBuffer->initialize();
        commandBuffer->recordAll();
        it++;
      } else {
        commandBuffers.erase(it);
      }
    }
  }
}

}