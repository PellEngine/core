#ifndef _PELLENGINE_ECS_COMPONENT_MANAGER_H_
#define _PELLENGINE_ECS_COMPONENT_MANAGER_H_

#include <unordered_map>
#include <array>
#include <memory>
#include "types.h"
#include "component_array.h"
#include <ctti/type_id.hpp>

namespace pellengine {

class ComponentManager {
 public:
  ComponentManager() {}
  ~ComponentManager() {}

  ComponentManager(const ComponentManager&) = delete;
  ComponentManager& operator=(const ComponentManager&) = delete;

  template<typename T>
  void registerComponent() {
    ctti::type_id_t name = ctti::type_id<T>();
    componentTypes.insert({name, nextComponentType});
    componentArrays.insert({name, std::make_shared<ComponentArray<T>>()});
    nextComponentType++;
  }

  template<typename T>
  ComponentType getComponentType() {
    ctti::type_id_t name = ctti::type_id<T>();
    return componentTypes[name];
  }

  template<typename T>
  void addComponent(Entity entity, T component) {
    getComponentArray<T>()->insertData(entity, component);
  }

  template<typename T>
  void removeComponent(Entity entity) {
    getComponentArray<T>()->removeData(entity);
  }

  template<typename T>
  T& getComponent(Entity entity) {
    return getComponentArray<T>()->getData(entity);
  }

  void entityDestroyed(Entity entity) {
    for(auto const& pair : componentArrays) {
      auto const& component = pair.second;
      component->entityDestroyed(entity);
    }
  }

 private:
  std::unordered_map<ctti::type_id_t, ComponentType> componentTypes;
  std::unordered_map<ctti::type_id_t, std::shared_ptr<ComponentArrayBase>> componentArrays;
  std::array<Signature, MAX_ENTITIES> signatures;
  ComponentType nextComponentType = 0;

  template<typename T>
  std::shared_ptr<ComponentArray<T>> getComponentArray() {
    ctti::type_id_t name = ctti::type_id<T>();
    return std::static_pointer_cast<ComponentArray<T>>(componentArrays[name]);
  }
};

}

#endif