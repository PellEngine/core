#ifndef _PELLENGINE_ECS_SYSTEM_MANAGER_H_
#define _PELLENGINE_ECS_SYSTEM_MANAGER_H_

#include <set>
#include <memory>
#include <unordered_map>
#include "types.h"
#include <ctti/type_id.hpp>
#include <algorithm>

namespace pellengine {

class System {
 public:
  virtual ~System() = default;
  std::set<Entity> entities;
  virtual void entityInserted(Entity entity) = 0;
  virtual void entityErased(Entity entity) = 0;
};

class SystemManager {
 public:
  SystemManager() {}
  ~SystemManager() {}

  SystemManager(const SystemManager&) = delete;
  SystemManager& operator=(const SystemManager&) = delete;

  template<typename T>
  void registerSystem(std::shared_ptr<T> system) {
    ctti::type_id_t name = ctti::type_id<T>();
    systems.insert({name, system});
  }

  template<typename T>
  void setSignature(Signature signature) {
    ctti::type_id_t name = ctti::type_id<T>();
    signatures.insert({name, signature});
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
        system->entityInserted(entity);
      } else {
        if(std::find(system->entities.begin(), system->entities.end(), entity) != system->entities.end()) {
          system->entityErased(entity);
        }
        system->entities.erase(entity);
      }
    }
  }

 private:
  std::unordered_map<ctti::type_id_t, Signature> signatures;
  std::unordered_map<ctti::type_id_t, std::shared_ptr<System>> systems;
};

}

#endif