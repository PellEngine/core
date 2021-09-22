#ifndef _PELLENGINE_GRAPHICS_CLEAR_COMMAND_BUFFER_H_
#define _PELLENGINE_GRAPHICS_CLEAR_COMMAND_BUFFER_H_

#include "src/pellengine/graphics/window.h"

namespace pellengine {

class ClearCommandBuffer {
 public:
  ClearCommandBuffer(std::shared_ptr<Window> window);
  ~ClearCommandBuffer() {}

  ClearCommandBuffer(const ClearCommandBuffer&) = delete;
  ClearCommandBuffer& operator=(const ClearCommandBuffer&) = delete;

  std::vector<VkCommandBuffer>& getCommandBuffers() {
    return commandBuffers;
  }

  void initialize();
  void recordAll();
  void terminate();

 private:
  std::shared_ptr<Window> window;
  bool initialized = false;
  std::vector<VkCommandBuffer> commandBuffers;
};

}

#endif