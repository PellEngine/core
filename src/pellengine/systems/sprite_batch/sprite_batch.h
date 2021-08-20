#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/ecs/system_manager.h"
#include "src/pellengine/ecs/entity_component_system.h"
#include <memory>
#include <vector>
#include <unordered_map>
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

  std::vector<std::shared_ptr<CommandBuffer>>& getCommandBuffers() {
    return commandBuffers;
  }

 private:
  bool initialized = false;
  std::shared_ptr<Window> window;
  std::shared_ptr<EntityComponentSystem> ecs;
  std::vector<std::shared_ptr<SpriteBatchLayer>> layers;
  std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;
  std::unordered_map<Entity, uint32_t> entityToLayer;
};

}

#endif