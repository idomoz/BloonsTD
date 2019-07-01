//
// Created by Ido Mozes on 20/06/2019.
//

#ifndef SDL2_GAME_ENTITY_H
#define SDL2_GAME_ENTITY_H

#include <bitset>
#include <vector>
#include <memory>
#include "Component.h"

constexpr int maxComponents = 64;
typedef std::size_t EntityId;


class Entity {
    static EntityId getNewId() {
        static EntityId id_counter = 0;
        return id_counter++;
    }

    bool active = true;
    EntityId id;
    std::bitset<ComponentType::LENGTH> componentsMask;
    std::array<Component *, ComponentType::LENGTH> components;
public:
    Entity();

    ~Entity() = default;

    EntityId getId() { return id; }

    bool isActive() { return active; }

    void destroy() { active = false; }

    bool hasComponents(uint64_t mask) { return (componentsMask.to_ullong() & mask) == mask; }

    template<class T>
    T &getComponent() { return *(T *) (components[T::getComponentType()]); }

    template<typename T, typename ... Targs>
    Component &addComponent(Targs &&... args){
        T *c = new T(this, std::forward<Targs>(args)...);
        components[T::getComponentType()] = c;
        componentsMask[T::getComponentType()] = true;
        return *c;
    }
};

#endif //SDL2_GAME_ENTITY_H
