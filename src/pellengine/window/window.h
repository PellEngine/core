#ifndef _PELLENGINE_WINDOW_WINDOW_H_
#define _PELLENGINE_WINDOW_WINDOW_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <string>
#include <vector>
#include <optional>
#include <set>
#include <android/log.h>
#include <cstdint>
#include <algorithm>

#ifdef ANDROID
  #include <android/native_activity.h>
#endif

namespace pellengine {

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

class Window {
 public:
  Window(std::string name, bool enableValidationLayers);
  ~Window();

  Window(const Window&) = delete;
  Window &operator=(const Window&) = delete;

  void initialize();
  void terminate();

  bool isInitialized() {
    return initialized;
  }

  #ifdef ANDROID
    void setAndroidPlatformWindow(ANativeWindow* androidPlatformWindow) {
      this->androidPlatformWindow = androidPlatformWindow;
    }
  #endif

 private:
  std::string name;
  bool initialized;
  bool enableValidationLayers;

  VkInstance instance;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkSurfaceKHR surface;
  VkSwapchainKHR swapChain;

  VkQueue graphicsQueue;
  VkQueue presentQueue;

  #ifdef ANDROID
    ANativeWindow* androidPlatformWindow;
  #endif

  void createInstance();
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSwapChain();

  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  bool checkValidationLayerSupport();
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  bool isDeviceSuitable(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentMode);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

  const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
  const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

}

#endif