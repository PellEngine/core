#ifndef _PELLENGINE_WINDOW_WINDOW_H_
#define _PELLENGINE_WINDOW_WINDOW_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <string>
#include <vector>

namespace pellengine {

class Window {
 public:
  Window(std::string name);
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
  VkInstance instance;

  bool createInstance();
};

}

#endif