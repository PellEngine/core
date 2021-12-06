#include "app.h"

namespace pellengine {

App::App(std::string appName, bool debug) {
  window = std::make_shared<Window>(appName, debug);
  ecs = std::make_shared<EntityComponentSystem>();
  renderer = std::make_shared<Renderer>(window);

  #ifdef ANDROID
    touchSystem = std::make_shared<TouchSystemAndroid>();
  #endif
}

App::~App() {}

}