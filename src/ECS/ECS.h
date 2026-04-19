#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
    static int nextId;
};

template<typename T>
class Component : public IComponent {
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
    //vector index = component type id
    // pool index = entity id
    std::vector<IPool *> componentPool;
    // vector index = entity id
    std::vector<Signature> entityComponentSignature;

    std::unordered_map<std::type_index, System *> systems;

public:
    Registry() = default;
    Entity& CreateEntity();
    void KillEntity(const Entity& entity);

     
};

#endif //ECS_H
