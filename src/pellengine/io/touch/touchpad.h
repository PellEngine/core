#ifndef _PELLENGINE_IO_TOUCH_TOUCHPAD_H_
#define _PELLENGINE_IO_TOUCH_TOUCHPAD_H_

#include <glm/vec2.hpp>
#include "src/pellengine/io/touch/touch_event.h"

namespace pellengine {

struct TapState {
  bool tapped;
  glm::vec2 position;
};

class Touchpad {
 public:
  Touchpad();
  ~Touchpad();

  void onEvent(const TouchEvent& event);

  TapState& getTapState() {
    return tapState;
  }

 private:
  TapState tapState;

  void handleTapEvent(const TouchEvent& event);
};

}

#endif