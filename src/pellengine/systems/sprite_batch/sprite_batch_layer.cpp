#include "sprite_batch_layer.h"

namespace pellengine {
  
SpriteBatchLayer::SpriteBatchLayer(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> ecs, uint32_t zIndex) : window(window), ecs(ecs), zIndex(zIndex) {
  this->graphicsPipeline = std::make_shared<GraphicsPipeline>(window, ShaderConfiguration::test(), SpriteBatchVertex::getVertexConfiguration());
  this->commandBuffer = std::make_shared<SpriteBatchCommandBuffer>(window, PipelineConfiguration::generateGraphicsPipelineConfiguration(graphicsPipeline), &vertexBuffer, &indexBuffer, SPRITE_BATCH_MAX_SPRITES * 6);
  SwapChainRecreator::registerCommandBuffer(commandBuffer);
  SwapChainRecreator::registerGraphicsPipeline(graphicsPipeline);
}

SpriteBatchLayer::~SpriteBatchLayer() {}

void SpriteBatchLayer::initialize() {
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

  // Pre generate all the indices values
  for(int i=0;i<SPRITE_BATCH_MAX_SPRITES;i++) {
    int vertexOffset = i * 4;
    int indexOffset = i * 6;
    indices[indexOffset + 0] = vertexOffset + 0;
    indices[indexOffset + 1] = vertexOffset + 1;
    indices[indexOffset + 2] = vertexOffset + 2;
    indices[indexOffset + 3] = vertexOffset + 2;
    indices[indexOffset + 4] = vertexOffset + 3;
    indices[indexOffset + 5] = vertexOffset + 0;
  }

  commandBuffer->initialize();
  commandBuffer->recordAll();
}

void SpriteBatchLayer::terminate() {
  commandBuffer->terminate();
  terminateBuffer(window, &vertexBuffer);
  terminateBuffer(window, &indexBuffer);
  graphicsPipeline->terminate();
}

void SpriteBatchLayer::addSprite(Entity entity) {
  uint32_t newIndex = numSprites;
  entityToIndex[entity] = newIndex;
  loadVertexProperties(entity, newIndex);
  entities.insert(entity);
  numSprites++;
}

void SpriteBatchLayer::removeSprite(Entity entity) {
  
}

void SpriteBatchLayer::loadVertexProperties(Entity entity, uint32_t index) {
  Sprite& sprite = ecs->getComponent<Sprite>(entity);
  Transform& transform = ecs->getComponent<Transform>(entity);
  int offset = index * 4;
  vertices[offset + 0] = {{transform.position.x, transform.position.y}, sprite.color};
  vertices[offset + 1] = {{transform.position.x + transform.scale.x, transform.position.y}, sprite.color};
  vertices[offset + 2] = {{transform.position.x + transform.scale.x, transform.position.y + transform.scale.y}, sprite.color};
  vertices[offset + 3] = {{transform.position.x, transform.position.y + transform.scale.y}, sprite.color};
  commandBufferFresh.clear();
}

void SpriteBatchLayer::update(uint32_t imageIndex) {
  for(const Entity& entity : entities) {
    Sprite& sprite = ecs->getComponent<Sprite>(entity);
    if(sprite.dirty) {
      loadVertexProperties(entity, entityToIndex[entity]);
      sprite.dirty = false;
    }
  }

  if(!commandBufferFresh[imageIndex]) {
    void* vertexData;
    vkMapMemory(window->getInstance()->getDevice(), vertexBuffer.bufferMemory, 0, sizeof(SpriteBatchVertex) * vertices.size(), 0, &vertexData);
    memcpy(vertexData, vertices.data(), (size_t) sizeof(SpriteBatchVertex) * vertices.size());
    vkUnmapMemory(window->getInstance()->getDevice(), vertexBuffer.bufferMemory);

    void* indexData;
    vkMapMemory(window->getInstance()->getDevice(), indexBuffer.bufferMemory, 0, sizeof(uint16_t) * indices.size(), 0, &indexData);
    memcpy(indexData, indices.data(), (size_t) sizeof(uint16_t) * indices.size());
    vkUnmapMemory(window->getInstance()->getDevice(), indexBuffer.bufferMemory);

    commandBuffer->record(imageIndex);
    commandBufferFresh[imageIndex] = true;
  }
}

}