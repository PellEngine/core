#include "sprite_batch.h"

namespace pellengine {

void SpriteBatch::entityInserted(Entity entity) {
  addSprite(entity);
}

void SpriteBatch::entityErased(Entity entity) {
  Logger::println(INFO, "Entity erased!");
}

void SpriteBatch::update(uint32_t imageIndex) {
  for(const Entity& entity : entities) {
    Sprite& sprite = ecs->getComponent<Sprite>(entity);
    sprite.transform.position.x += 0.001;
    loadVertexProperties(entity, entityToIndexMap[entity]);
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

void SpriteBatch::addSprite(Entity entity) {
  uint32_t newIndex = numSprites;
  entityToIndexMap[entity] = newIndex;
  loadVertexProperties(entity, newIndex);
  numSprites++;
}

void SpriteBatch::loadVertexProperties(Entity entity, uint32_t index) {
  Sprite& sprite = ecs->getComponent<Sprite>(entity);

  int vertexOffset = index * 4;
  int indexOffset = index * 6;
  vertices[vertexOffset + 0] = {{-0.5f + sprite.transform.position.x, -0.5f + sprite.transform.position.y}, {1.0f, 0.0f, 0.0f, 1.0f}};
  vertices[vertexOffset + 1] = {{0.5f + sprite.transform.position.x, -0.5f + sprite.transform.position.y}, {0.0f, 1.0f, 0.0f, 1.0f}};
  vertices[vertexOffset + 2] = {{0.5f + sprite.transform.position.x, 0.5f + sprite.transform.position.y}, {0.0f, 0.0f, 1.0f, 1.0f}};
  vertices[vertexOffset + 3] = {{-0.5f + sprite.transform.position.x, 0.5f + sprite.transform.position.y}, {1.0f, 1.0f, 1.0f, 1.0f}};

  indices[indexOffset + 0] = vertexOffset + 0;
  indices[indexOffset + 1] = vertexOffset + 1;
  indices[indexOffset + 2] = vertexOffset + 2;
  indices[indexOffset + 3] = vertexOffset + 2;
  indices[indexOffset + 4] = vertexOffset + 3;
  indices[indexOffset + 5] = vertexOffset + 0;

  commandBufferFresh.clear();
}

}

// namespace pellengine {

// SpriteBatch::SpriteBatch(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> entityComponentSystem) : System(), window(window), entityComponentSystem(entityComponentSystem) {
//   graphicsPipeline = std::shared_ptr<GraphicsPipeline>(new GraphicsPipeline(window, ShaderConfiguration::test(), SpriteBatchVertex::getVertexConfiguration()));
//   commandBuffer = std::shared_ptr<SpriteBatchCommandBuffer>(new SpriteBatchCommandBuffer(window, PipelineConfiguration::generateGraphicsPipelineConfiguration(graphicsPipeline), vertexBuffer, SPRITE_BATCH_MAX_SPRITES * 3));
//   SwapChainRecreator::registerGraphicsPipeline(graphicsPipeline);
//   SwapChainRecreator::registerCommandBuffer(commandBuffer);
// }

// SpriteBatch::~SpriteBatch() {}

// void SpriteBatch::initialize() {
//   graphicsPipeline->initialize(); 
//   createVertexBuffer(window, vertexBuffer.get(), sizeof(SpriteBatchVertex) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
//   commandBuffer->initialize();
//   commandBuffer->recordAll();
// }

// void SpriteBatch::terminate() {
//   // commandBuffer->terminate();
//   terminateVertexBuffer(window, vertexBuffer.get());
//   graphicsPipeline->terminate();
// }

// void SpriteBatch::entityInserted(Entity entity) {
//   addSprite(entity);
// }

// void SpriteBatch::entityErased(Entity entity) {
//   removeSprite(entity);
// }

// void SpriteBatch::addSprite(Entity entity) {
//   uint32_t newIndex = numSprites;
//   entityToIndexMap[entity] = newIndex;
//   indexToEntityMap[newIndex] = entity;
//   loadVertexProperties(newIndex);
//   numSprites++;
// }

// void SpriteBatch::removeSprite(Entity entity) {
//   uint32_t index = entityToIndexMap[entity];
//   int offset = index * 3;
//   vertices[offset + 0] = {};
//   vertices[offset + 1] = {};
//   vertices[offset + 2] = {};
// }

// void SpriteBatch::loadVertexProperties(int index) {
//   Entity entity = indexToEntityMap[index];
//   Sprite& sprite = entityComponentSystem->getComponent<Sprite>(entity);

//   int offset = index * 3;
//   vertices[offset + 0] = {{0.0f + sprite.transform.position.x, -0.5f + sprite.transform.position.y}, {1.0f, 0.0f, 0.0f, 1.0f}};
//   vertices[offset + 1] = {{0.5f + sprite.transform.position.x, 0.5f + sprite.transform.position.y}, {0.0f, 1.0f, 0.0f, 1.0f}};
//   vertices[offset + 2] = {{-0.5f + sprite.transform.position.x, 0.5f + sprite.transform.position.y}, {0.0f, 0.0f, 1.0f, 1.0f}};
//   commandBufferFresh.clear();
// }

// void SpriteBatch::draw(uint32_t imageIndex) {
//   // If it is neccerssary, reload the vertex buffer
//   if(commandBufferFresh[imageIndex]) {
//     void* data;
//     vkMapMemory(window->getInstance()->getDevice(), vertexBuffer->vertexBufferMemory, 0, sizeof(SpriteBatchVertex) * vertices.size(), 0, &data);
//     memcpy(data, vertices.data(), (size_t) sizeof(SpriteBatchVertex) * vertices.size());
//     vkUnmapMemory(window->getInstance()->getDevice(), vertexBuffer->vertexBufferMemory);
//     commandBuffer->record(imageIndex);
//     commandBufferFresh[imageIndex] = true;
//   }
// }

// }