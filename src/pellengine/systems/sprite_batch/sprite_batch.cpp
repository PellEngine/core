#include "sprite_batch.h"

namespace pellengine {

SpriteBatch::SpriteBatch(
  std::shared_ptr<Window> window,
  std::shared_ptr<EntityComponentSystem> ecs,
  std::shared_ptr<Renderer2D> renderer
) : window(window), ecs(ecs), renderer(renderer) {
  this->uniformBuffer = std::make_shared<UniformBuffer>(window, sizeof(SpriteBatchUniformBufferObject));
  SwapChainRecreator::registerUniformBuffer(this->uniformBuffer);
}
SpriteBatch::~SpriteBatch() {}

/*
  ----------------------------------------------------
  Setup the graphics resources for sprite batch layers
  ----------------------------------------------------
*/
void SpriteBatch::initialize() {
  //Initialize uniform buffer
  uniformBuffer->initialize();

  // Initialize all the layers
  if(initialized) return;
  initialized = true;
  for(std::shared_ptr<SpriteBatchLayer>& layer : layers) {
    layer->initialize();
  }
}

void SpriteBatch::terminate() {
  if(!initialized) return;
  initialized = false;
  for(std::shared_ptr<SpriteBatchLayer>& layer : layers) {
    layer->terminate();
  }
  uniformBuffer->terminate();
}

/*
  -----------------------------------
  Handle sprite creation and deletion
  -----------------------------------
*/
void SpriteBatch::entityInserted(Entity entity) {
  // Find first sprite batch layer with room
  bool added = false;
  uint32_t index = 0;
  Sprite& sprite = ecs->getComponent<Sprite>(entity);

  for(std::shared_ptr<SpriteBatchLayer> layer : layers) {
    if(layer->hasRoom()) {
      if((!layer->hasSpriteSheet(sprite.spriteSheet) && layer->hasTextureRoom()) || layer->hasSpriteSheet(sprite.spriteSheet)) {
        layer->addSprite(entity);
        entityToLayer.insert({ entity, index });
        added = true;
        break;
      }
    }
    index++;
  }

  // Create new sprite batch layer and add entity
  if(!added) {
    std::shared_ptr<SpriteBatchLayer> layer = std::make_shared<SpriteBatchLayer>(window, ecs, uniformBuffer);
    if(initialized) {
      layer->initialize();
    }
    
    layers.push_back(layer);
    renderer->addRenderable(layer);
    layer->addSprite(entity);
    entityToLayer.insert({ entity, layers.size() - 1 }); 
  }
}

void SpriteBatch::entityErased(Entity entity) {
  int index = entityToLayer[entity];
  std::shared_ptr<SpriteBatchLayer> layer = layers[index];
  if(layer.get() != nullptr) {
    entityToLayer.erase(entity);
    layer->removeSprite(entity);
  } else {
    Logger::println(LogSeverity::ERROR, "Layer is a nullptr");
  }
}

/*
  ----------------------------
  Update invalid sprite layers
  ----------------------------
*/
void SpriteBatch::updateLayers(uint32_t imageIndex) {
  // Update uniform buffer
  SpriteBatchUniformBufferObject ubo{};
  ubo.proj = glm::ortho(0.0f, (float)window->getSwapChainExtent().width, 0.0f, (float)window->getSwapChainExtent().height);
  void* data = uniformBuffer->map(imageIndex);
  memcpy(data, &ubo, sizeof(ubo));
  uniformBuffer->unmap(imageIndex, data);

  for(std::shared_ptr<SpriteBatchLayer> layer : layers) {
    layer->update(imageIndex);
  }
}

}