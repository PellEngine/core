#ifndef _PELLENGINE_ECS_COMPONENT_ARRAY_H_
#define _PELLENGINE_ECS_COMPONENT_ARRAY_H_

#include <array>
#include <unordered_map>
#include "types.h"

namespace pellengine {

class ComponentArrayBase {
 public:
  virtual ~ComponentArrayBase() = default;
  virtual void entityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public ComponentArrayBase {
 public:
  void insertData(Entity entity, T component) {
    size_t newIndex = size;
    entityToIndexMap[entity] = newIndex;
    indexToEntityMap[newIndex] = entity;
    componentArray[newIndex] = component;
    size++;
  }

  void removeData(Entity entity) {
    size_t removedEntityIndex = entityToIndexMap[entity];
    size_t lastElementIndex = size - 1;
    componentArray[removedEntityIndex] = componentArray[lastElementIndex];

    Entity lastEntity = indexToEntityMap[lastElementIndex];
    entityToIndexMap[lastEntity] = removedEntityIndex;
    indexToEntityMap[removedEntityIndex] = lastEntity;

    entityToIndexMap.erase(entity);
    indexToEntityMap.erase(lastElementIndex);

    size--;
  }

  T& getData(Entity entity) {
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