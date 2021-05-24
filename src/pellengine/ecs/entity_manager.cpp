#include "entity_manager.h"

namespace pellengine {

EntityManager::EntityManager() {
  for(Entity entity = 0;entity<MAX_ENTITIES;entity++) {
    availableEntities.push(entity);
  }
}

EntityManager::~EntityManager() {}

Entity EntityManager::createEntity() {
  assert(activeEntityCount < MAX_ENTITIES && "Too many entities.");

  Entity id = availableEntities.front();
  availableEntities.pop();
  activeEntityCount++;

  return id;
}

void EntityManager::destroyEntity(Entity entity) {
  assert(entity < MAX_ENTITIES && "Entity out of range.");
  signatures[entity].reset();
  availableEntities.push(entity);
  activeEntityCount--;
}

void EntityManager::setSignature(Entity entity, Signature signature) {
  assert(entity < MAX_ENTITIES && "Entity out of range.");
  signatures[entity] = signature;
}

Signature EntityManager::getSignature(Entity entity) {
  assert(entity < MAX_ENTITIES && "Entity out of range.");
  return signatures[entity];
}

}