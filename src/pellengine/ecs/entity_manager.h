#ifndef _PELLENGINE_ECS_ENTITY_MANAGER_H_
#define _PELLENGINE_ECS_ENTITY_MANAGER_H_

#include <array>
#include "types.h"

namespace pellengine {

class EntityManager {
 public:
  EntityManager() {}
  ~EntityManager() {}

  EntityManager(const EntityManager&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;

  Entity createEntity() {
    Entity newEntity = nextEntity;
    nextEntity++;
    return newEntity;
  }

  void destroyEntity(Entity entity) {
    signatures[entity].reset();
  }

  void setSignature(Entity entity, Signature signature) {
    signatures[entity] = signature;
  }

  Signature getSignature(Entity entity) {
    return signatures[entity];
  }

 private:
  Entity nextEntity = 0;
  std::array<Signature, MAX_ENTITIES> signatures;
};

}

#endif