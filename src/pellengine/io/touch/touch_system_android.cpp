#ifdef ANDROID
#include "touch_system_android.h"

namespace pellengine {

TouchSystemAndroid::TouchSystemAndroid() {}
TouchSystemAndroid::~TouchSystemAndroid() {}

int32_t TouchSystemAndroid::onInputEvent(AInputEvent* event) {
  int32_t type = AInputEvent_getType(event);
  if(type == AINPUT_EVENT_TYPE_MOTION) {
    int32_t action = AMotionEvent_getAction(event);
    uint32_t eventFlags = action & AMOTION_EVENT_ACTION_MASK;

    TouchEvent touchEvent;
    switch(eventFlags) {
      case AMOTION_EVENT_ACTION_DOWN:
      case AMOTION_EVENT_ACTION_POINTER_DOWN:
        touchEvent.type = TouchType::DOWN;
        break;
      case AMOTION_EVENT_ACTION_UP:
      case AMOTION_EVENT_ACTION_POINTER_UP:
        touchEvent.type = TouchType::UP;
        break;
      case AMOTION_EVENT_ACTION_MOVE:
        touchEvent.type = TouchType::MOVE;
        break;
      case AMOTION_EVENT_ACTION_CANCEL:
        touchEvent.type = TouchType::CANCELLED;
      default:
        return 0;
    }

    touchEvent.numTouches = AMotionEvent_getPointerCount(event);
    for(int32_t i=0;i<touchEvent.numTouches;i++) {
      TouchPoint& point = touchEvent.points[i];
      point.id = AMotionEvent_getPointerId(event, i);
      point.pos.x = AMotionEvent_getX(event, i);
      point.pos.y = AMotionEvent_getY(event, i);
    }

    eventQueue.push(touchEvent);

    return 1;
  }

  return 0;
}

}

#endif