#ifndef _PELLENGINE_RENDER_SYSTEM_SPRITE_BATCH_H_
#define _PELLENGINE_RENDER_SYSTEM_SPRITE_BATCH_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <pellengine/graphics/buffer.h>
#include <pellengine/graphics/graphics_pipeline.h>
#include <pellengine/graphics/window.h>
#include <pellengine/graphics/swapchain_recreator.h>
#include <pellengine/ecs/system_manager.h>
#include <pellengine/ecs/entity_component_system.h>
#include <pellengine/components/sprite.h>
#include <pellengine/io/logger.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <array>
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
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
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

class SpriteBatch : public System {
 public:
  SpriteBatch(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> ecs) {
    this->window = window;
    this->ecs = ecs;
    this->graphicsPipeline = std::shared_ptr<GraphicsPipeline>(new GraphicsPipeline(window, ShaderConfiguration::test(), SpriteBatchVertex::getVertexConfiguration()));
    this->commandBuffer = std::shared_ptr<SpriteBatchCommandBuffer>(new SpriteBatchCommandBuffer(window, PipelineConfiguration::generateGraphicsPipelineConfiguration(graphicsPipeline), &vertexBuffer, &indexBuffer, SPRITE_BATCH_MAX_SPRITES * 6));
    SwapChainRecreator::registerCommandBuffer(commandBuffer);
    SwapChainRecreator::registerGraphicsPipeline(graphicsPipeline);
  }
  ~SpriteBatch() {}

  void initialize() {
    graphicsPipeline->initialize();
    createBuffer(
      window,
      &vertexBuffer,
      sizeof(SpriteBatchVertex) * vertices.size(),
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_SHARING_MODE_EXCLUSIVE,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    createBuffer(
      window,
      &indexBuffer,
      sizeof(uint16_t) * indices.size(),
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_SHARING_MODE_EXCLUSIVE,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    commandBuffer->initialize();
    commandBuffer->recordAll();
  }

  void terminate() {
    commandBuffer->terminate();
    terminateBuffer(window, &vertexBuffer);
    terminateBuffer(window, &indexBuffer);
    graphicsPipeline->terminate();
  }

  void update(uint32_t imageIndex);
  void entityInserted(Entity entity) override;
  void entityErased(Entity entity) override;

  std::shared_ptr<SpriteBatchCommandBuffer> getCommandBuffer() {
    return commandBuffer;
  }
  
 private:
  std::shared_ptr<Window> window;
  std::shared_ptr<EntityComponentSystem> ecs;
  std::shared_ptr<SpriteBatchCommandBuffer> commandBuffer;
  std::shared_ptr<GraphicsPipeline> graphicsPipeline;
  Buffer vertexBuffer;
  Buffer indexBuffer;
  std::array<SpriteBatchVertex, SPRITE_BATCH_MAX_SPRITES * 4> vertices;
  std::array<uint16_t, SPRITE_BATCH_MAX_SPRITES * 6> indices;
  std::unordered_map<Entity, uint32_t> entityToIndexMap;
  std::unordered_map<uint32_t, bool> commandBufferFresh;
  uint32_t numSprites = 0;

  void addSprite(Entity entity);
  void loadVertexProperties(Entity entity, uint32_t index);
};

}

#endif

// #include <pellengine/graphics/graphics_pipeline.h>
// #include <pellengine/graphics/vertex_buffer.h>
// #include <pellengine/graphics/swapchain_recreator.h>
// #include <pellengine/ecs/system.h>
// #include <pellengine/ecs/entity_component_system.h>
// #include <pellengine/components/sprite.h>
// #include <pellengine/systems/sprite_batch_command_buffer.h>
// #include <pellengine/vulkan/vulkan_wrapper.h>
// #include <glm/vec2.hpp>
// #include <glm/vec4.hpp>
// #include <memory>
// #include <vector>
// #include <array>
// #include <unordered_map>

// namespace pellengine {

// const uint32_t SPRITE_BATCH_MAX_SPRITES = 4096;

// struct SpriteBatchVertex {
//   glm::vec2 pos;
//   glm::vec4 color;

//   static VkVertexInputBindingDescription getBindingDescription() {
//     VkVertexInputBindingDescription bindingDescription{};
//     bindingDescription.binding = 0;
//     bindingDescription.stride = sizeof(SpriteBatchVertex);
//     bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//     return bindingDescription;
//   }

//   static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
//     std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

//     // Position attribute
//     attributeDescriptions[0].binding = 0;
//     attributeDescriptions[0].location = 0;
//     attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
//     attributeDescriptions[0].offset = offsetof(SpriteBatchVertex, pos);

//     // Color attribute
//     attributeDescriptions[1].binding = 0;
//     attributeDescriptions[1].location = 1;
//     attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//     attributeDescriptions[1].offset = offsetof(SpriteBatchVertex, color);

//     return attributeDescriptions;
//   }

//   static VertexConfiguration getVertexConfiguration() {
//     VertexConfiguration configuration{};
//     configuration.bindingDescriptions.push_back(SpriteBatchVertex::getBindingDescription());
//     configuration.attributeDescriptions = SpriteBatchVertex::getAttributeDescriptions();
//     return configuration;
//   }
// };

// class SpriteBatch : public System {
//  public:
//   SpriteBatch(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> entityComponentSystem);
//   ~SpriteBatch();

//   SpriteBatch(const SpriteBatch&) = delete;
//   SpriteBatch& operator=(const SpriteBatch&) = delete;

//   void initialize();
//   void terminate();
//   void draw(uint32_t imageIndex);

//   void entityInserted(Entity entity) override;
//   void entityErased(Entity entity) override;

//   std::shared_ptr<SpriteBatchCommandBuffer> getCommandBuffer() {
//     return commandBuffer;
//   }

//  private:
//   std::shared_ptr<VertexBuffer> vertexBuffer;
//   std::shared_ptr<Window> window;
//   std::shared_ptr<EntityComponentSystem> entityComponentSystem;
//   std::shared_ptr<GraphicsPipeline> graphicsPipeline;
//   std::shared_ptr<SpriteBatchCommandBuffer> commandBuffer;

//   std::array<SpriteBatchVertex, SPRITE_BATCH_MAX_SPRITES * 3> vertices;
//   std::unordered_map<Entity, uint32_t> entityToIndexMap; 
//   std::unordered_map<uint32_t, Entity> indexToEntityMap;
//   std::unordered_map<uint32_t, bool> commandBufferFresh;
//   uint32_t numSprites;

//   void addSprite(Entity entity);
//   void removeSprite(Entity entity);
//   void loadVertexProperties(int index);
// };

// }

// #endif