#ifndef _PELLENGINE_ECS_SYSTEM_H_
#define _PELLENGINE_ECS_SYSTEM_H_

#include <pellengine/ecs/entity.h>
#include <pellengine/ecs/component.h>
#include <set>
#include <memory>
#include <unordered_map>

namespace pellengine {

class System {
 public:
  std::set<Entity> entities;
};

class SystemManager {
 public:
  SystemManager(const SystemManager&) = delete;
  SystemManager& operator=(const SystemManager&) = delete;

  template<typename T>
  std::shared_ptr<T> registerSystem() {
    const char* typeName = typeid(T).name();
    assert(systems.find(typeName) == systems.end() && "This system already exists.");
    auto system = std::make_shared<T>();
    systems.insert({typeName, system});
    return system;
  }

  template<typename T>
  void setSignature(Signature signature) {
    const char* typeName = typeid(T).name();
    assert(systems.find(typeName) != systems.end() && "This system doesn't exists.");
    signatures.insert({typeName, signature});
  }

  void entityDestroyed(Entity entity) {
    for(auto const& pair : systems) {
      auto const& system = pair.second;
      system->entities.erase(entity);
    }
  }

  void entitySignatureChanged(Entity entity, Signature entitySignature) {
    for(auto const& pair : systems) {
      auto const& type = pair.first;
      auto const& system = pair.second;
      auto const& systemSignature = signatures[type];

      if((entitySignature & systemSignature) == systemSignature) {
        system->entities.insert(entity);
      } else {
        system->entities.erase(entity);
      }
    }
  }

 private:
  std::unordered_map<const char*, Signature> signatures;
  std::unordered_map<const char*, std::shared_ptr<System>> systems;
};

}

#endif