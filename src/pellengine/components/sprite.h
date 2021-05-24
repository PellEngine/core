#ifndef _PELLENGINE_COMPONENTS_SPRITE_H_
#define _PELLENGINE_COMPONENTS_SPRITE_H_

#include <pellengine/components/transform.h>

namespace pellengine {

struct Sprite {
  Transform transform;
  bool outdated;
};

}

#endif