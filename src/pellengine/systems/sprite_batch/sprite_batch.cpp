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

}

void SpriteBatch::terminate() {

}

/*
  -----------------------------------
  Handle sprite creation and deletion
  -----------------------------------
*/
void SpriteBatch::entityInserted(Entity entity) {

}

void SpriteBatch::entityErased(Entity entity) {

}

/*
  ----------------------------
  Update invalid sprite layers
  ----------------------------
*/
void updateLayers(uint32_t imageIndex) {

}

}