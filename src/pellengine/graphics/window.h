#ifndef _PELLENGINE_GRAPHICS_WINDOW_H_
#define _PELLENGINE_GRAPHICS_WINDOW_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <pellengine/graphics/instance.h>
#include <string>
#include <vector>

namespace pellengine {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

const int MAX_FRAMES_IN_FLIGHT = 3;

class Window {
 public:
  Window(const std::string name, bool enableValidationLayers);
  ~Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  void initialize();
  void terminate();
  void recreateSwapChain();
  void terminateSwapChain();

  std::vector<VkImageView>& getSwapChainImageViews() {
    return swapChainImageViews;
  }

  std::vector<VkImage>& getSwapChainImages() {
    return swapChainImages;
  }

  VkExtent2D getSwapChainExtent() {
    return swapChainExtent;
  }

  VkCommandPool getCommandPool() {
    return commandPool;
  }

  VkRenderPass getRenderPass() {
    return renderPass;
  }

  VkSwapchainKHR getSwapchain() {
    return swapChain;
  }

  std::vector<VkFramebuffer>& getSwapChainFrameBuffers() {
    return swapChainFrameBuffers;
  }

  Instance* getInstance() {
    return instance;
  }

  bool isInitialized() {
    return initialized;
  }

 private:
  Instance* instance;

  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkImageView> swapChainImageViews;

  VkRenderPass renderPass;
  std::vector<VkFramebuffer> swapChainFrameBuffers;
  VkCommandPool commandPool;

  void createSwapChain();
  void createImageViews();
  void createRenderPass();
  void createFramebuffers();
  void createCommandPool();

  bool initialized;
  bool enableValidationLayers;

  SwapChainSupportDetails querySwapChainSupport(Instance* instance);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};

}

#endif