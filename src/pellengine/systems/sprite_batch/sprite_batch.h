#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_H_

#include <pellengine/graphics/window.h>
#include <pellengine/ecs/system_manager.h>
#include <pellengine/ecs/entity_component_system.h>
#include <memory>
#include <vector>
#include "sprite_batch_layer.h"

namespace pellengine {

class SpriteBatch : public System {
 public:
  SpriteBatch(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> ecs);
  ~SpriteBatch();

  SpriteBatch(const SpriteBatch&) = delete;
  SpriteBatch& operator=(const SpriteBatch&) = delete;

  void initialize();
  void terminate();
  void updateLayers(uint32_t imageIndex);
  void entityInserted(Entity entity) override;
  void entityErased(Entity entity) override;

 private:
  std::shared_ptr<Window> window;
  std::shared_ptr<EntityComponentSystem> ecs;
  std::vector<SpriteBatchLayer> layers;
};

}

#endif