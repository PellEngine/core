#ifndef _PELLENGINE_ECS_ENTITY_COMPONENT_SYSTEM_H_
#define _PELLENGINE_ECS_ENTITY_COMPONENT_SYSTEM_H_

#include <unordered_map>
#include <vector>
#include <memory>
#include "component_mananger.h"
#include "entity_manager.h"
#include "system_manager.h"

namespace pellengine {

class EntityComponentSystem {
 public:
  EntityComponentSystem() {}
  ~EntityComponentSystem() {}

  EntityComponentSystem(const EntityComponentSystem&) = delete;
  EntityComponentSystem& operator=(const EntityComponentSystem&) = delete;

  void initialize() {
    componentManager = std::make_unique<ComponentManager>();
    entityManager = std::make_unique<EntityManager>();
    systemManager = std::make_unique<SystemManager>();
  }

  template<typename T>
  void registerComponent() {
    componentManager->registerComponent<T>();
  }

  template<typename T>
  void addComponent(Entity entity, T component) {
    componentManager->addComponent<T>(entity, component);
    auto signature = entityManager->getSignature(entity);
    signature.set(componentManager->getComponentType<T>(), true);
    entityManager->setSignature(entity, signature);
    systemManager->entitySignatureChanged(entity, signature);
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
  ComponentType getComponentType() {
    return componentManager->getComponentType<T>();
  }

  template<typename T>
  T& getComponent(Entity entity) {
    return componentManager->getComponent<T>(entity);
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
  void registerSystem(std::shared_ptr<T> system) {
    systemManager->registerSystem<T>(system);
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