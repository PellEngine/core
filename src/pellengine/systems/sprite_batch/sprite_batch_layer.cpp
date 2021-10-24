#include "sprite_batch_layer.h"

namespace pellengine {

SpriteBatchLayer::SpriteBatchLayer(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> ecs, std::shared_ptr<SpriteBatchPipeline> pipeline) : window(window), ecs(ecs), pipeline(pipeline) {}
SpriteBatchLayer::~SpriteBatchLayer() {
  terminate();
}

void SpriteBatchLayer::initialize() {
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

  // Load all vertex properties as buffers are cleared when
  // app is put in background
  for(const Entity& entity : entities) {
    loadVertexProperties(entity, entityToIndex[entity]);
  }
}

void SpriteBatchLayer::terminate() {
  terminateBuffer(window, &vertexBuffer);
  terminateBuffer(window, &indexBuffer);
}

void SpriteBatchLayer::addSprite(Entity entity) {
  uint32_t newIndex = numSprites;
  entityToIndex[entity] = newIndex;
  indexToEntity[newIndex] = entity;
  loadVertexProperties(entity, newIndex);
  entities.insert(entity);
  numSprites++;
}

void SpriteBatchLayer::removeSprite(Entity entity) {
  // Find the sprite index for this sprite and the last sprite. Move the last sprite back the empty spot.
  Entity lastEntity = indexToEntity[numSprites - 1];
  uint32_t entityIndex = entityToIndex[entity];

  indexToEntity[entityIndex] = lastEntity;
  entityToIndex[lastEntity] = entityIndex;

  int offset = (numSprites - 1) * 4;
  vertices[offset + 0] = {};
  vertices[offset + 1] = {};
  vertices[offset + 2] = {};
  vertices[offset + 3] = {};

  loadVertexProperties(lastEntity, entityIndex);
}

void SpriteBatchLayer::loadVertexProperties(Entity entity, uint32_t index) {
  Sprite& sprite = ecs->getComponent<Sprite>(entity);
  Transform& transform = ecs->getComponent<Transform>(entity);
  int offset = index * 4;
  vertices[offset + 0] = {{transform.position.x, transform.position.y, sprite.zIndex}, sprite.color, {1.0f, 0.0f}};
  vertices[offset + 1] = {{transform.position.x  + transform.scale.x, transform.position.y, sprite.zIndex}, sprite.color, {0.0f, 0.0f}};
  vertices[offset + 2] = {{transform.position.x + transform.scale.x, transform.position.y + transform.scale.y, sprite.zIndex}, sprite.color, {0.0f, 1.0f}};
  vertices[offset + 3] = {{transform.position.x, transform.position.y + transform.scale.y, sprite.zIndex}, sprite.color, {1.0f, 1.0f}};
  outdatedBuffer = true;
}

void SpriteBatchLayer::update(uint32_t imageIndex) {
  for(const Entity& entity : entities) {
    Sprite& sprite = ecs->getComponent<Sprite>(entity);

    if(sprite.dirty) {
      loadVertexProperties(entity, entityToIndex[entity]);
      sprite.dirty = false;
    }
  }

  if(outdatedBuffer) {
    void* vertexData = vertexBuffer.map(window, 0, sizeof(SpriteBatchVertex) * vertices.size());
    memcpy(vertexData, vertices.data(), (size_t) sizeof(SpriteBatchVertex) * vertices.size());
    vertexBuffer.unmap(window);

    void* indexData = indexBuffer.map(window, 0, sizeof(uint16_t) * indices.size());
    memcpy(indexData, indices.data(), (size_t) sizeof(uint16_t) * indices.size());
    indexBuffer.unmap(window);

    outdatedBuffer = false;
  }
}

void SpriteBatchLayer::render(VkCommandBuffer& commandBuffer, uint32_t imageIndex) {
  VkBuffer vertexBuffers[] = {this->vertexBuffer.buffer};
  VkDeviceSize offsets[] = {0};
  
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipeline());
  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineLayout(), 0, 1, &pipeline->getDescriptorSets()[0][imageIndex], 0, nullptr);
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
  vkCmdBindIndexBuffer(commandBuffer, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
  vkCmdDrawIndexed(commandBuffer, SPRITE_BATCH_MAX_SPRITES * 6, 1, 0, 0, 0);
}

}