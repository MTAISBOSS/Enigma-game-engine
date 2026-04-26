#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include "../Logger/Logger.h"
#include <string>

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
    static int nextId;
};

template<typename T>
class Component : public IComponent {
public:
    static int GetId() {
        static auto id = nextId++;
        return id;
    }

private:
    int id;
};

class Entity {
private:
    int id;
public:
    Entity(int id) : id(id) {};

    Entity(const Entity &entity) = default;

    int GetId() const;

    Entity &operator=(const Entity &entity) = default;

    bool operator==(const Entity &entity) const { return id == entity.GetId(); }

    bool operator!=(const Entity &entity) const { return id != entity.GetId(); }

    bool operator>(const Entity &entity) const { return id > entity.GetId(); }

    bool operator<(const Entity &entity) const { return id < entity.GetId(); }
};

class System {
private:
    Signature componentSignature;
    std::vector<Entity> entities;
public:
    System() = default;

    virtual ~System() = default;

    void AddEntityToSystem(Entity entity);

    void RemoveEntityToSystem(Entity entity);

    std::vector<Entity> GetSystemEntities() const;

    const Signature &GetComponentSignature() const;

    template<typename TComponent>
    void RequireComponent();
};

template<typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

class IPool {
public:
    virtual ~IPool() {}
};

template<typename TPool>
class Pool : public IPool {
private:
    std::vector<TPool> data;

public:
    Pool(int size = 100) {
        data.resize(size);
    }

    virtual ~Pool() = default;

    bool isEmpty() {
        return data.empty();
    }

    int GetSize() {
        return data.size();
    }

    void Resize(int size) {
        data.resize(size);
    }

    void Clear() {
        data.clear();
    }

    void Add(TPool object) {
        data.push_back(object);
    }

    void Set(int index, TPool object) {
        data[index] = object;
    }

    TPool &Get(int index) {
        return static_cast<TPool>(data[index]);
    }

    TPool &operator[](unsigned int index) {
        return data[index];
    }
};

class Registry {
private:
    int numberOfEntities = 0;
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;
    //vector index = component type id
    // pool index = entity id
    std::vector<std::shared_ptr<IPool>> componentPools;
    // vector index = entity id
    std::vector<Signature> entityComponentSignatures;

    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

public:
    //Registry() = default;
    Registry() {
        Logger::Log("Registry constructor");
    };

    ~Registry() {
        Logger::Log("Registry destructor");
    }

    Entity CreateEntity();

    void KillEntity(const Entity &entity);

    void Update();

    void AddEntityToSystems(Entity entity);

    template<typename T, typename ...TArgs>
    void AddComponent(Entity entity, TArgs &&...args);

    template<typename T>
    void RemoveComponent(Entity entity);

    template<typename T>
    bool HasComponent(Entity entity);

    template<typename TSystem, typename ...TArgs>
    void AddSystem(TArgs &&...args);

    template<typename TSystem>
    void RemoveSystem();

    template<typename TSystem>
    bool HasSystem() const;

    template<typename TSystem>
    TSystem &GetSystem() const;

};

template<typename TSystem>
TSystem &Registry::GetSystem() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

template<typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem>
void Registry::RemoveSystem() {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template<typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&... args) {
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename T>
bool Registry::HasComponent(Entity entity) {
    const int componentId = Component<T>::GetId();
    const int entityId = entity.GetId();

    return entityComponentSignatures[entityId].test(componentId);
}

template<typename T>
void Registry::RemoveComponent(Entity entity) {
    const int componentId = Component<T>::GetId();
    const int entityId = entity.GetId();

    entityComponentSignatures[entityId].set(componentId, false);
}

template<typename TComponent, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs &&... args) {
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    if (componentId >= componentPools.size()) {
        componentPools.resize(componentId + 1, nullptr);
    }

    if (!componentPools[componentId]) {
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentId] = newComponentPool;
    }

    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(
            componentPools[componentId]);

    if (entityId >= componentPool->GetSize()) {
        componentPool->Resize(numberOfEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);

    entityComponentSignatures[entityId].set(componentId);

    Logger::Log(
            "Component id: "
            + std::to_string(componentId)
            + "was added to Entity id "
            + std::to_string(entityId));
}

#endif //ECS_H
