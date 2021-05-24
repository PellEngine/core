#ifndef _PELLENGINE_ECS_COMPONENT_MANAGER_H_
#define _PELLENGINE_ECS_COMPONENT_MANAGER_H_

#include <pellengine/ecs/component.h>
#include <pellengine/ecs/component_array.h>
#include <unordered_map>
#include <memory>

namespace pellengine {

class ComponentManager {
 public:
  ComponentManager(const ComponentManager&) = delete;
  ComponentManager& operator=(const ComponentManager&) = delete;

  template<typename T>
  void registerComponent() {
    const char* typeName = typeid(T).name();
    assert(componentTypes.find(typeName) == componentTypes.end() && "This component type already exists.");
    componentTypes.insert({typeName, nextComponentType});
    componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()})
    nextComponentType++;
  }

  template<typename T>
  ComponentType getComponentType() {
    const char* typeName = typeid(T).name();
    assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");
    return componentTypes[typeName];
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
  ComponentType nextComponentType;
  std::unordered_map<const char*, ComponentType> componentTypes;
  std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays;

  template<typename T>
  std::shared_ptr<ComponentArray<T>> getComponentArray() {
    const char* typeName = typeid(T).name();
    assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");
    return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
  }
};

}

#endif