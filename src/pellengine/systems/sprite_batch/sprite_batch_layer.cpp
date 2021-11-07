#include "sprite_batch_layer.h"

namespace pellengine {

SpriteBatchLayer::SpriteBatchLayer(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> ecs, std::shared_ptr<UniformBuffer> uniformBuffer) : window(window), ecs(ecs) {
  allocator = std::make_shared<DescriptorAllocator>(window);
  pipeline = std::make_shared<SpriteBatchPipeline>(window, uniformBuffer, allocator, SPRITE_BATCH_MAX_TEXTURES);
  SwapChainRecreator::registerGraphicsPipeline(this->pipeline);
}
SpriteBatchLayer::~SpriteBatchLayer() {
  terminate();
}

void SpriteBatchLayer::initialize() {
  if(initialized) return;

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

  pipeline->initialize();
  initialized = true;
}

void SpriteBatchLayer::terminate() {
  if(!initialized) return;
  pipeline->terminate();
  allocator->terminate();
  terminateBuffer(window, &vertexBuffer);
  terminateBuffer(window, &indexBuffer);
  initialized = false;
}

void SpriteBatchLayer::addSprite(Entity entity) {
  // Check if we need to load a new sprite sheet
  Sprite& sprite = ecs->getComponent<Sprite>(entity);
  if(!hasSpriteSheet(sprite.spriteSheet) && !hasTextureRoom()) {
    throw std::runtime_error("Added sprite to full sprite batch layer");
  }

  // Load in new sprite sheet
  if(!hasSpriteSheet(sprite.spriteSheet) && hasTextureRoom()) {
    pipeline->addSpriteSheet(sprite.spriteSheet);
  }

  // Find TextureID
  auto it = std::find(pipeline->getSpriteSheets().begin(), pipeline->getSpriteSheets().end(), sprite.spriteSheet);
  int textureID = std::distance(pipeline->getSpriteSheets().begin(), it);
  entityToTextureID[entity] = textureID;

  uint32_t newIndex = numSprites;
  entityToIndex[entity] = newIndex;
  indexToEntity[newIndex] = entity;
  if(initialized) {
    loadVertexProperties(entity, newIndex);
  }
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

  if(initialized) {
    loadVertexProperties(lastEntity, entityIndex);
  }
}

void SpriteBatchLayer::loadVertexProperties(Entity entity, uint32_t index) {
  Sprite& sprite = ecs->getComponent<Sprite>(entity);
  Transform& transform = ecs->getComponent<Transform>(entity);
  int offset = index * 4;

  std::array<glm::vec2, 4>& texCoords = sprite.spriteSheet->getTexCoords(sprite.spritePos.x, sprite.spritePos.y);

  vertices[offset + 0] = SpriteBatchVertex{
    .pos = glm::vec3(transform.position.x, transform.position.y, sprite.zIndex),
    .color = sprite.color,
    .texCoord = texCoords[0],
    .textureID = entityToTextureID[entity]
  };

  vertices[offset + 1] = SpriteBatchVertex{
    .pos = glm::vec3(transform.position.x  + transform.scale.x, transform.position.y, sprite.zIndex),
    .color = sprite.color,
    .texCoord = texCoords[1],
    .textureID = entityToTextureID[entity]
  };

  vertices[offset + 2] = SpriteBatchVertex{
    .pos = glm::vec3(transform.position.x + transform.scale.x, transform.position.y + transform.scale.y, sprite.zIndex),
    .color = sprite.color,
    .texCoord = texCoords[2],
    .textureID = entityToTextureID[entity]
  };

  vertices[offset + 3] = SpriteBatchVertex{
    .pos = glm::vec3(transform.position.x, transform.position.y + transform.scale.y, sprite.zIndex),
    .color = sprite.color,
    .texCoord = texCoords[3],
    .textureID = entityToTextureID[entity]
  };

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