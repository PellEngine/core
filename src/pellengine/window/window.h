#ifndef _PELLENGINE_WINDOW_WINDOW_H_
#define _PELLENGINE_WINDOW_WINDOW_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <string>
#include <vector>

namespace pellengine {

class Window {
 public:
  Window(std::string name, bool enableValidationLayers);
  ~Window();

  Window(const Window&) = delete;
  Window &operator=(const Window&) = delete;

  bool initialize();
  bool terminate();

  bool isInitialized() {
    return initialized;
  }

 private:
  std::string name;
  bool initialized;
  bool enableValidationLayers;

  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;

  bool createInstance();
  bool setupDebugMessenger();

  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  bool checkValidationLayerSupport();
  
  const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
};

}

#endif