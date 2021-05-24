#ifndef _PELLENGINE_ECS_ENTITY_MANAGER_H_
#define _PELLENGINE_ECS_ENTITY_MANAGER_H_

#include <pellengine/ecs/entity.h>
#include <pellengine/ecs/component.h>
#include <queue>
#include <array>

namespace pellengine {

class EntityManager {
 public:
  EntityManager();
  ~EntityManager();

  EntityManager(const EntityManager&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;

  Entity createEntity();
  void destroyEntity(Entity entity);
  void setSignature(Entity entity, Signature signature);
  Signature getSignature(Entity entity);

 private:
  std::queue<Entity> availableEntities;
  std::array<Signature, MAX_ENTITIES> signatures;
  uint32_t activeEntityCount;
};

}

#endif