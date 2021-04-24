#include "window.h"

namespace pellengine {

Window::Window(const std::string name) {
  this->name = name;
  this->initialized = false;
}

Window::~Window() {
  this->terminate();
}

bool Window::initialize() {
  if(!InitVulkan()) {
    throw std::runtime_error("Vulkan is not avaiable on this device.");
    return false;
  }

  if(this->initialized) {
    return true;
  }

  if(!this->createInstance()) {
    return false;
  }

  this->initialized = true;
  return true;
}

bool Window::terminate() {
  vkDestroyInstance(instance, nullptr);
  this->initialized = false;

  return true;
}

bool Window::createInstance() {
  // Create basic instance structs
  VkApplicationInfo appInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = nullptr,
    .apiVersion = VK_MAKE_VERSION(1,0,0),
    .pApplicationName = this->name.c_str(),
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "PellEngine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
  };

  VkInstanceCreateInfo createInfo = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = nullptr,
    .pApplicationInfo = &appInfo,
    .flags = 0,
  };

  // Specify required extensions for each platform
  std::vector<const char*> instanceExtensions;
    
  #ifdef ANDROID
    instanceExtensions.push_back("VK_KHR_surface");
    instanceExtensions.push_back("VK_KHR_android_surface");
  #endif

  // Make sure that the extensions are available
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

  bool extensionsPresent = true;
  for(const char* requiredExtension : instanceExtensions) {
    bool found = false;
    for(const VkExtensionProperties& extension : extensions) {
      if(strcmp(extension.extensionName, requiredExtension) == 0) {
        found = true;
        break;
      }
    }

    if(!found) {
      extensionsPresent = false;
      break;
    }
  }

  if(!extensionsPresent) {
    throw std::runtime_error("This device doesn't support all the required extensions.");
    return false;
  }

  createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
  createInfo.ppEnabledExtensionNames = instanceExtensions.data();
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;

  if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create vulkan instance.");
    return false;
  }

  return true;
}

}