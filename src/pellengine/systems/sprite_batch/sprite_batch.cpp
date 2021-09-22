#include "sprite_batch.h"

namespace pellengine {

SpriteBatch::SpriteBatch(std::shared_ptr<Window> window, std::shared_ptr<EntityComponentSystem> ecs) : window(window), ecs(ecs) {}
SpriteBatch::~SpriteBatch() {}

/*
  ----------------------------------------------------
  Setup the graphics resources for sprite batch layers
  ----------------------------------------------------
*/
void SpriteBatch::initialize() {
  // Initialize all the layers
  if(initialized) return;
  initialized = true;
  for(std::shared_ptr<SpriteBatchLayer> layer : layers) {
    layer->initialize();
  }
}

void SpriteBatch::terminate() {
  if(!initialized) return;
  initialized = false;
  for(std::shared_ptr<SpriteBatchLayer> layer : layers) {
    layer->terminate();
  }
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

  for(std::shared_ptr<SpriteBatchLayer> layer : layers) {
    if(layer->hasRoom()) {
      layer->addSprite(entity);
      entityToLayer.insert({ entity, index });
      added = true;
      break;
    }
    index++;
  }

  // Create new sprite batch layer and add entity
  if(!added) {
    std::shared_ptr<SpriteBatchLayer> layer = std::make_shared<SpriteBatchLayer>(window, ecs);
    if(initialized) {
      layer->initialize();
    }
    
    layers.push_back(layer);
    commandBuffers.push_back(layer->getCommandBuffer());
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
  for(std::shared_ptr<SpriteBatchLayer> layer : layers) {
    layer->update(imageIndex);
  }
}

}