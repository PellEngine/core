#ifndef _PELLENGINE_GRAPHICS_INSTANCE_H_
#define _PELLENGINE_GRAPHICS_INSTANCE_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include "src/pellengine/io/logger.h"
#include <string>
#include <vector>
#include <optional>
#include <set>

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
  virtual ~Instance();

  Instance(const Instance&) = delete;
  Instance& operator=(const Instance&) = delete;

  void initialize();
  void terminate();
  
  virtual int getWidth() = 0;
  virtual int getHeight() = 0;
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  bool isInitialized() {
    return initialized;
  }

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

 protected:
  VkInstance instance;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkSurfaceKHR surface;

  VkQueue graphicsQueue;
  VkQueue presentQueue;

  bool enableValidationLayers;
  bool initialized;
  std::string name;

  void createInstance();
  void setupDebugMessenger();
  virtual void createSurface() = 0;
  void pickPhysicalDevice();
  void createLogicalDevice();

  bool checkValidationLayerSupport();
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  bool isDeviceSuitable(VkPhysicalDevice device);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  virtual void getInstanceExtensions(std::vector<const char*>& instanceExtensions) = 0;

  const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
  const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

}

#endif