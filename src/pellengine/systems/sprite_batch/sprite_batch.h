#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/swapchain_recreator.h"
#include "src/pellengine/graphics/uniform_buffer.h"
#include "src/pellengine/ecs/system_manager.h"
#include "src/pellengine/ecs/entity_component_system.h"
#include "src/pellengine/renderers/renderer2D/renderer2D.h"
#include <unordered_map>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "sprite_batch_layer.h"

namespace pellengine {

struct SpriteBatchUniformBufferObject {
  glm::mat4 proj;
};

class SpriteBatch : public System {
 public:
  SpriteBatch(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> ecs, std::shared_ptr<Renderer2D> renderer);
  ~SpriteBatch();

  SpriteBatch(const SpriteBatch&) = delete;
  SpriteBatch& operator=(const SpriteBatch&) = delete;

  void initialize();
  void terminate();
  void updateLayers(uint32_t imageIndex);
  void entityInserted(Entity entity) override;
  void entityErased(Entity entity) override;

 private:
  bool initialized = false;
  std::shared_ptr<Window> window;
  std::shared_ptr<EntityComponentSystem> ecs;
  std::shared_ptr<Renderer2D> renderer;
  std::vector<std::shared_ptr<SpriteBatchLayer>> layers;
  std::unordered_map<Entity, uint32_t> entityToLayer;
  std::shared_ptr<UniformBuffer> uniformBuffer;
};

}

#endif