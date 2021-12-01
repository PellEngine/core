#ifdef ANDROID
#ifndef _PELLENGINE_IO_TOUCH_TOUCH_SYSTEM_ANDROID_
#define _PELLENGINE_IO_TOUCH_TOUCH_SYSTEM_ANDROID_

#include <android/native_activity.h>
#include "touch_system.h"

namespace pellengine {

class TouchSystemAndroid : public TouchSystem {
 public:
  TouchSystemAndroid();
  ~TouchSystemAndroid();

  TouchSystemAndroid(const TouchSystemAndroid&) = delete;
  TouchSystemAndroid& operator=(const TouchSystemAndroid&) = delete;

  int32_t onInputEvent(AInputEvent* event);
};

}

#endif
#endif