#ifndef _PELLENGINE_GRAPHICS_INSTANCE_H_
#define _PELLENGINE_GRAPHICS_INSTANCE_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <pellengine/io/logger.h>
#include <string>
#include <vector>
#include <optional>
#include <set>

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

class Instance {
 public:
  Instance(const std::string name, bool enableValidationLayers);
  ~Instance();

  Instance(const Instance&) = delete;
  Instance& operator=(const Instance&) = delete;

  void initialize();
  void terminate();
  
  int getWidth();
  int getHeight();
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  bool isInitialized() {
    return initialized;
  }

  #ifdef ANDROID
    void setAndroidPlatformWindow(ANativeWindow* window) {
      androidPlatformWindow = window;
    }
  #endif

  VkInstance getInstance() {
    return instance;
  }

  VkPhysicalDevice getPhysicalDevice() {
    return physicalDevice;
  }

  VkDevice getDevice() {
    return device;
  }

  VkSurfaceKHR getSurface() {
    return surface;
  }

  VkQueue getGraphicsQueue() {
    return graphicsQueue;
  }

  VkQueue getPresentQueue() {
    return presentQueue;
  }

 private:
  VkInstance instance;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkSurfaceKHR surface;

  VkQueue graphicsQueue;
  VkQueue presentQueue;

  #ifdef ANDROID
    ANativeWindow* androidPlatformWindow;
  #endif

  bool enableValidationLayers;
  bool initialized;
  std::string name;

  void createInstance();
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();

  bool checkValidationLayerSupport();
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  bool isDeviceSuitable(VkPhysicalDevice device);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);

  const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
  const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

}

#endif