#ifndef _PELLENGINE_WINDOW_WINDOW_H_
#define _PELLENGINE_WINDOW_WINDOW_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <string>
#include <vector>
#include <optional>

namespace pellengine {

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;

  bool isComplete() {
    return graphicsFamily.has_value();
  }
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

 private:
  std::string name;
  bool initialized;
  bool enableValidationLayers;

  VkInstance instance;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkQueue graphicsQueue;

  void createInstance();
  void setupDebugMessenger();
  void pickPhysicalDevice();
  void createLogicalDevice();

  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  bool checkValidationLayerSupport();
  bool isDeviceSuitable(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
};

}

#endif