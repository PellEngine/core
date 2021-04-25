#include "window.h"
#include <iostream>

namespace pellengine {

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
  void* pUserData) {

  std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(
  VkInstance instance,
  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
  const VkAllocationCallbacks* pAllocator,
  VkDebugUtilsMessengerEXT* pDebugMessenger) {
  
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

  if(func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(
  VkInstance instance,
  VkDebugUtilsMessengerEXT debugMessenger,
  const VkAllocationCallbacks* pAllocator) {
  
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

  if(func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

Window::Window(const std::string name, bool enableValidationLayers) {
  this->name = name;
  this->initialized = false;
  this->enableValidationLayers = enableValidationLayers;
}

Window::~Window() {
  this->terminate();
}

bool Window::initialize() {
  if(!InitVulkan()) {
    throw std::runtime_error("Vulkan is not available on this device.");
    return false;
  }

  if(this->initialized) {
    return true;
  }

  if(!this->createInstance()) {
    return false;
  }

  if(!this->setupDebugMessenger()) {
    return false;
  }

  this->initialized = true;
  return true;
}

bool Window::terminate() {

  if(enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
  }

  vkDestroyInstance(instance, nullptr);
  
  this->initialized = false;
  return true;
}

bool Window::createInstance() {
  if(enableValidationLayers && !checkValidationLayerSupport()) {
    throw std::runtime_error("Validation layers are requested, but your device doesn't support them.");
  }

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

  if(enableValidationLayers) {
    instanceExtensions.push_back("VK_EXT_debug_utils");
  }

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

  // Setup debug validation layers
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

  if(enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
  }

  if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create vulkan instance.");
    return false;
  }

  return true;
}

bool Window::setupDebugMessenger() {
  if(!this->enableValidationLayers) return false;
  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  this->populateDebugMessengerCreateInfo(createInfo);
  if(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("Failed to set up debug messenger");
    return false;
  }

  return true;
}

void Window::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity 
    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  
  createInfo.messageType
    = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
    | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;
}

bool Window::checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for(const char* layerName : validationLayers) {
    bool layerFound = false;

    for(const VkLayerProperties& layerProperties : availableLayers) {
      if(strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if(!layerFound) {
      return false;
    }
  }

  return true;
}

}