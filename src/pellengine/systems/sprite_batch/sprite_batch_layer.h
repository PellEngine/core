#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_LAYER_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_LAYER_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <pellengine/graphics/buffer.h>
#include <pellengine/graphics/graphics_pipeline.h>
#include <pellengine/graphics/window.h>
#include <pellengine/graphics/swapchain_recreator.h>
#include <pellengine/ecs/entity_component_system.h>
#include <pellengine/components/transform.h>
#include <pellengine/components/sprite.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <vector>
#include <array>
#include <set>
#include <unordered_map>
#include "sprite_batch_command_buffer.h"

namespace pellengine {

const uint32_t SPRITE_BATCH_MAX_SPRITES = 4096;

struct SpriteBatchVertex {
  glm::vec2 pos;
  glm::vec4 color;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(SpriteBatchVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
  }

  static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

    // Position attribute
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(SpriteBatchVertex, pos);

    // Color attribute
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(SpriteBatchVertex, color);

    return attributeDescriptions;
  }

  static VertexConfiguration getVertexConfiguration() {
    VertexConfiguration configuration{};
    configuration.bindingDescriptions.push_back(SpriteBatchVertex::getBindingDescription());
    configuration.attributeDescriptions = SpriteBatchVertex::getAttributeDescriptions();
    return configuration;
  }
};

class SpriteBatchLayer {
 public:
  SpriteBatchLayer(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> ecs, uint32_t zIndex);
  ~SpriteBatchLayer();

  SpriteBatchLayer(const SpriteBatchLayer&) = delete;
  SpriteBatchLayer& operator=(const SpriteBatchLayer&) = delete;

  void initialize();
  void terminate();
  void update(uint32_t imageIndex);
  void addSprite(Entity entity);
  void removeSprite(Entity entity);

 private:
  std::shared_ptr<Window> window;
  std::shared_ptr<EntityComponentSystem> ecs;
  std::shared_ptr<SpriteBatchCommandBuffer> commandBuffer;
  std::shared_ptr<GraphicsPipeline> graphicsPipeline;
  std::set<Entity> entities;
  Buffer vertexBuffer;
  Buffer indexBuffer;
  std::array<SpriteBatchVertex, SPRITE_BATCH_MAX_SPRITES * 4> vertices;
  std::array<uint16_t, SPRITE_BATCH_MAX_SPRITES * 6> indices;
  std::unordered_map<uint32_t, bool> commandBufferFresh;
  std::unordered_map<Entity, uint32_t> entityToIndex;
  uint32_t numSprites = 0;
  uint32_t zIndex;

  void loadVertexProperties(Entity entity, uint32_t index);
};

}

#endif