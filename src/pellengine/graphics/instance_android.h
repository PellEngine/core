#ifdef ANDROID
#ifndef _PELLENGINE_GRAPHICS_INSTANCE_ANDROID_H_
#define _PELLENGINE_GRAPHICS_INSTANCE_ANDROID_H_

#include "instance.h"
#include <android/native_activity.h>

namespace pellengine {

class InstanceAndroid : public Instance {
 public:
  InstanceAndroid(const std::string name, bool enableValidationLayers);
  ~InstanceAndroid();
  
  int getWidth() override;
  int getHeight() override;

  void setPlatformWindow(ANativeWindow* window) {
    platformWindow = window;
  }

 protected:
  void createSurface() override;
  void getInstanceExtensions(std::vector<const char*>& instanceExtensions) override;

 private:
  ANativeWindow* platformWindow;
};

}

#endif
#endif