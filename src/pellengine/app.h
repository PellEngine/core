#ifndef _PELLENGINE_APP_H_
#define _PELLENGINE_APP_H_

#include "src/pellengine/graphics/window.h"

namespace pellengine {

class App {
 public:
  App() {}
  virtual ~App() {}

 protected:
  std::shared_ptr<Window> window;
};

}

#endif