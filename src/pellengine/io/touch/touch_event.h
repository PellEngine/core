#ifndef _PELLENGINE_IO_TOUCH_TOUCH_EVENT_H_
#define _PELLENGINE_IO_TOUCH_TOUCH_EVENT_H_

#include <glm/vec2.hpp>
#include <stdint.h>

namespace pellengine {

enum TouchType {
  DOWN,
  MOVE,
  UP,
  CANCELLED,
  INVALID
};

struct TouchPoint {
  uint32_t id;
  glm::vec2 pos;
};

struct TouchEvent {
  TouchType type = INVALID;
  size_t numTouches = 0;
  static const uint32_t maxPoints = 8;
  TouchPoint points[maxPoints];
};

}

#endif