#ifndef _PELLENGINE_ECS_ENTITY_COMPONENT_SYSTEM_H_
#define _PELLENGINE_ECS_ENTITY_COMPONENT_SYSTEM_H_

#include <pellengine/ecs/component_manager.h>
#include <pellengine/ecs/entity_manager.h>
#include <pellengine/ecs/system.h>
#include <pellengine/ecs/entity.h>
#include <memory>

namespace pellengine {

class EntityComponentSytem {
 public:
  EntityComponentSytem(const EntityComponentSytem&) = delete;
  EntityComponentSytem& operator=(const EntityComponentSytem&) = delete;

  void initialize() {
    componentManager = std::make_unique<ComponentManager>();
    entityManager = std::make_unique<EntityManager>();
    systemManager = std::make_unique<SystemManager>();
  }

  Entity createEntity() {
    return entityManager->createEntity();
  }

  void destroyEntity(Entity entity) {
    entityManager->destroyEntity(entity);
    componentManager->entityDestroyed(entity);
    systemManager->entityDestroyed(entity);
  }

  template<typename T>
  void registerComponent() {
    componentManager->registerComponent<T>();
  }

  template<typename T>
  void removeComponent(Entity entity) {
    componentManager->removeComponent<T>(entity);
    
    auto signature = entityManager->getSignature(entity);
    signature.set(componentManager->getComponentType<T>(), false);
    entityManager->setSignature(entity, signature);
    systemManager->entitySignatureChanged(entity, signature);
  }

  template<typename T>
  T& getComponent(Entity entity) {
    return componentManager->getComponent<T>(entity);
  }

  template<typename T>
  ComponentType getComponentType() {
    return componentManager->getComponentType<T>();
  }

  template<typename T>
  std::shared_ptr<T> registerSystem() {
    return systemManager->registerSystem<T>();
  }

  template<typename T>
  void setSystemSignature(Signature signature) {
    systemManager->setSignature<T>(signature);
  }

 private:
  std::unique_ptr<ComponentManager> componentManager;
  std::unique_ptr<EntityManager> entityManager;
  std::unique_ptr<SystemManager> systemManager;
};

}

#endif