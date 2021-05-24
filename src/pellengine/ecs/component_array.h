#ifndef _PELLENGINE_ECS_COMPONENT_ARRAY_H_
#define _PELLENGINE_ECS_COMPONENT_ARRAY_H_

#include <pellengine/ecs/entity.h>
#include <array>
#include <unordered_map>

namespace pellengine {

class IComponentArray {
 public:
  virtual ~IComponentArray() = default;
  virtual void entityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
 public:
  ComponentArray(const ComponentArray&) = delete;
  ComponentArray& operator=(const ComponentArray&) = delete;

  void insertData(Entity entity, T component) {
    assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added more than once.");

    size_t newIndex = size;
    entityToIndexMap[entity] = newIndex;
    indexToEntityMap[newIndex] = entity;
    componentArray[newIndex] = component;
    size++;
  }

  void removeData(Entity entity) {
    assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing component that doesnt exists.");

    // Copy element from last index to deleted elements index
    size_t removedEntityIndex = entityToIndexMap[entity];
    size_t lastElementIndex = size - 1;
    componentArray[removedEntityIndex] = componentArray[lastElementIndex];

    // Update mappings
    Entity lastEntity = indexToEntityMap[lastElementIndex];
    entityToIndexMap[lastEntity] = removedEntityIndex;
    indexToEntityMap[removedEntityIndex] = lastEntity;

    entityToIndexMap.erase(entity);
    indexToEntityMap.erase(lastElementIndex);

    size--;
  }

  T& getData(Entity entity) {
    assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Component doesn't exists");
    return componentArray[entityToIndexMap[entity]];
  }

  void entityDestroyed(Entity entity) override {
    if(entityToIndexMap.find(entity) != entityToIndexMap.end()) {
      removeData(entity);
    }
  }

 private:
  std::array<T, MAX_ENTITIES> componentArray;
  std::unordered_map<Entity, size_t> entityToIndexMap;
  std::unordered_map<size_t, Entity> indexToEntityMap;
  size_t size;
};

}

#endif