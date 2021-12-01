#include "touchpad.h"

namespace pellengine {

Touchpad::Touchpad() {}
Touchpad::~Touchpad() {}

void Touchpad::onEvent(const TouchEvent& event) {
  handleTapEvent(event);
}

void Touchpad::handleTapEvent(const TouchEvent& event) {
  if(event.numTouches > 1 || event.numTouches == 0) {
    tapState.tapped = false;
  } else {
    if(event.type == TouchType::DOWN || event.type == TouchType::MOVE) {
      tapState.tapped = true;
      tapState.position = event.points[0].pos;
    } else {
      tapState.tapped = false;
    }
  }
}

}