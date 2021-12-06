#ifndef _PELLENGINE_APP_H_
#define _PELLENGINE_APP_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/renderer.h"
#include "src/pellengine/ecs/entity_component_system.h"
#include "src/pellengine/io/touch/touch_system.h"
#include "src/pellengine/io/touch/touch_system_android.h"
#include <string>

namespace pellengine {

class App {
 public:
  App(std::string appName, bool debug);
  virtual ~App();

 protected:
  std::shared_ptr<Window> window;
  std::shared_ptr<EntityComponentSystem> ecs;
  std::shared_ptr<Renderer> renderer;
  std::shared_ptr<TouchSystem> touchSystem;
};

}

#include "app.ipp"
#endif