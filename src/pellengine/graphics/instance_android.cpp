#ifdef ANDROID
#include "instance_android.h"

namespace pellengine {

InstanceAndroid::InstanceAndroid(const std::string name, bool enableValidationLayers) : Instance(name, enableValidationLayers) {}
InstanceAndroid::~InstanceAndroid() {
  platformWindow = nullptr;
}

int InstanceAndroid::getWidth() {
  return ANativeWindow_getWidth(platformWindow);
}

int InstanceAndroid::getHeight() {
  return ANativeWindow_getHeight(platformWindow);
}

void InstanceAndroid::createSurface() {
  VkAndroidSurfaceCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.window = platformWindow;

  if(vkCreateAndroidSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create android surface.");
  }
}

void InstanceAndroid::getInstanceExtensions(std::vector<const char*>& instanceExtensions) {
  instanceExtensions.push_back("VK_KHR_surface");
  instanceExtensions.push_back("VK_KHR_android_surface");
}

}

#endif