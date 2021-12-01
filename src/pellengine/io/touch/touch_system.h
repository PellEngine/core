#ifndef _PELLENGINE_IO_TOUCH_TOUCH_SYSTEM_H_
#define _PELLENGINE_IO_TOUCH_TOUCH_SYSTEM_H_

#include "src/pellengine/io/safe_queue.h"
#include <glm/vec2.hpp>
#include <queue>
#include <unordered_map>
#include <ctti/type_id.hpp>
#include "touch_event.h"

namespace pellengine {

class TouchSystem {
 public:
  TouchSystem() {}
  virtual ~TouchSystem() {}

  TouchSystem(const TouchSystem&) = delete;
  TouchSystem& operator=(const TouchSystem&) = delete;

  SafeQueue<TouchEvent>& getEventQueue() {
    return eventQueue;
  }

 protected:
  SafeQueue<TouchEvent> eventQueue;
};

};

#endif