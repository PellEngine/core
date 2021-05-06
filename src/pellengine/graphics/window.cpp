#include "window.h"
#include <iostream>

namespace pellengine {

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
  void* pUserData) {
  
  Logger::vprintln(ERROR, "Validation layer: %s", pCallbackData->pMessage);
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

void Window::initialize() {
  if(!InitVulkan()) {
    throw std::runtime_error("Vulkan is not available on this device.");
  }

  if(this->initialized) {
    return;
  }

  createInstance();
  setupDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
  createImageViews();

  this->initialized = true;
}

void Window::terminate() {
  for(auto imageView : swapChainImageViews) {
    vkDestroyImageView(device, imageView, nullptr);
  }

  vkDestroySwapchainKHR(device, swapChain, nullptr);
  vkDestroyDevice(device, nullptr);

  if(enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
  }

  vkDestroySurfaceKHR(instance, surface, nullptr);
  vkDestroyInstance(instance, nullptr);

  #ifdef ANDROID
    this->androidPlatformWindow = nullptr;
  #endif
  
  this->initialized = false;
}

void Window::createInstance() {
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
  }

  createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
  createInfo.ppEnabledExtensionNames = instanceExtensions.data();

  // Setup debug validation layers
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

  if(enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    populateDebugMessengerCreateInfo(debugCreateInfo);  
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
  }

  if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create vulkan instance.");
  }
}

void Window::setupDebugMessenger() {
  if(!this->enableValidationLayers) return;
  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  this->populateDebugMessengerCreateInfo(createInfo);

  // if(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
  //   throw std::runtime_error("Failed to set up debug messenger");
  // }
}

void Window::createSurface() {
  #ifdef ANDROID
    VkAndroidSurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.window = this->androidPlatformWindow;

    if(vkCreateAndroidSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create android surface.");
    }
  #endif
}

void Window::pickPhysicalDevice() {
  uint32_t deviceCount;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

  if(deviceCount == 0) {
    throw std::runtime_error("Failed to find GPU with vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  // Pick the first suitable device
  for(const auto& device : devices) {
    if(isDeviceSuitable(device)) {
      this->physicalDevice = device;
      break;
    }
  }

  if(this->physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to find suitable GPU!");
  }
}

void Window::createLogicalDevice() {
  QueueFamilyIndices indices = this->findQueueFamilies(this->physicalDevice);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

  float queuePriority = 1.0;
  for(uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();

  if(this->enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create logical device!");
  }

  vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
  vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &presentQueue);
}

void Window::createSwapChain() {
  SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

  if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
  uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

  if(indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create swap chain");
  }

  vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);

  swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

  swapChainImageFormat = surfaceFormat.format;
  swapChainExtent = extent;
}

void Window::createImageViews() {
  swapChainImageViews.resize(swapChainImages.size());

  for(size_t i=0;i<swapChainImages.size();i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainImages[i];
    
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImageFormat;

    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if(vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create image views.");
    }
  }
}

/*
  -------------------------------
  Initialization helper functions
  -------------------------------
*/


bool Window::isDeviceSuitable(VkPhysicalDevice device) {
  QueueFamilyIndices indices = findQueueFamilies(device);

  bool extensionsSupported = checkDeviceExtensionSupport(device);
  bool swapChainAdequate = false;

  if(extensionsSupported) {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
    swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
  }

  return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

QueueFamilyIndices Window::findQueueFamilies(VkPhysicalDevice device) {
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i=0;
  for(const auto& queueFamily : queueFamilies) {
    if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

    if(presentSupport) {
      indices.presentFamily = i;
    }

    if(indices.isComplete()) {
      break;
    }

    i++;
  }

  return indices;
}

SwapChainSupportDetails Window::querySwapChainSupport(VkPhysicalDevice device) {
  SwapChainSupportDetails details;

  // Get the capabilities
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  // Get the supported formats
  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if(formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
  }

  // Get the supported present modes
  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

  if(presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR Window::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
  for(const auto& format : availableFormats) {
    if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR Window::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
  for(const auto& presentMode : availablePresentModes) {
    if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return presentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Window::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
  if(capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    int width = 0;
    int height = 0;

    #ifdef ANDROID
      width = ANativeWindow_getWidth(this->androidPlatformWindow);
      height = ANativeWindow_getHeight(this->androidPlatformWindow);
    #endif

    VkExtent2D actualExtent = {
      static_cast<uint32_t>(width),
      static_cast<uint32_t>(height)
    };

    actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
  }
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

bool Window::checkDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

  for(const auto& extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

}