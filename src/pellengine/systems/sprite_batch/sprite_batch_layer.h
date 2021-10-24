#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_LAYER_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_LAYER_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include "src/pellengine/graphics/buffer.h"
#include "src/pellengine/graphics/window.h"
#include "src/pellengine/ecs/entity_component_system.h"
#include "src/pellengine/components/transform.h"
#include "src/pellengine/components/sprite.h"
#include "src/pellengine/renderers/renderer2D/renderer2D.h"
#include "sprite_batch_pipeline.h"

namespace pellengine {

const uint32_t SPRITE_BATCH_MAX_SPRITES = 4096;

class SpriteBatchLayer : public Renderable2D {
 public:
  SpriteBatchLayer(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> ecs, std::shared_ptr<SpriteBatchPipeline> pipeline);
  virtual ~SpriteBatchLayer();

  SpriteBatchLayer(const SpriteBatchLayer&) = delete;
  SpriteBatchLayer& operator=(const SpriteBatchLayer&) = delete;

  void initialize();
  void terminate();
  void update(uint32_t imageIndex);
  void addSprite(Entity entity);
  void removeSprite(Entity entity);

  bool hasRoom() {
    return numSprites < SPRITE_BATCH_MAX_SPRITES;
  }

  void render(VkCommandBuffer& commandBuffer, uint32_t imageIndex) override;
 
 private:
  std::shared_ptr<Window> window;
  std::shared_ptr<EntityComponentSystem> ecs;
  std::shared_ptr<SpriteBatchPipeline> pipeline;
  std::set<Entity> entities;
  Buffer vertexBuffer;
  Buffer indexBuffer;
  bool outdatedBuffer = true;
  std::array<SpriteBatchVertex, SPRITE_BATCH_MAX_SPRITES * 4> vertices;
  std::array<uint16_t, SPRITE_BATCH_MAX_SPRITES * 6> indices;
  std::unordered_map<Entity, uint32_t> entityToIndex;
  std::unordered_map<uint32_t, Entity> indexToEntity;
  uint32_t numSprites = 0;

  void loadVertexProperties(Entity entity, uint32_t index);
};

}

#endif