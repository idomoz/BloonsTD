//
// Created by Ido Mozes on 20/06/2019.
//

#ifndef SDL2_GAME_ENTITY_H
#define SDL2_GAME_ENTITY_H

#include <bitset>
#include <vector>
#include <cmath>
#include <memory>
#include <optional>
#include "Component.h"

constexpr uint64_t createMask(std::initializer_list<int> types) noexcept {
    uint64_t mask = 0;
    for (int bit : types) {
        mask |= (uint64_t) pow(2, bit);
    }
    return mask;
}

class Entity {

    std::bitset<ComponentType::LENGTH> componentsMask;
    std::unique_ptr<Component> components[ComponentType::LENGTH];
public:

//    bool hasComponents(uint64_t mask) { return (componentsMask.to_ullong() & mask) == mask; }

//    template<class T>
//    T &getComponent() { return *(T *) (components[T::getComponentType()].get()); }

    template<class ... T>
    std::optional<std::tuple<T &...>> getComponents() {
        uint64_t mask = createMask({T::getComponentType()...});
        if ((componentsMask.to_ullong() & mask) == mask) {
            return std::tuple<T &...>((*(T *) components[T::getComponentType()].get())...);
        }
        return std::nullopt;
    }

    template<class T>
    T * getComponent() {
        if (auto &c =components[T::getComponentType()])
            return (T*)c.get();
        return nullptr;
    }

    template<typename T, typename ... Targs>
    Component &addComponent(Targs &&... args) {
        T *c = new T(this, std::forward<Targs>(args)...);
        components[T::getComponentType()] = std::move(std::unique_ptr<Component>(c));
        componentsMask[T::getComponentType()] = true;
        return *c;
    }

    template<typename T>
    void removeComponent() {
        components[T::getComponentType()].release();
        componentsMask[T::getComponentType()] = false;
    }

};

#endif //SDL2_GAME_ENTITY_H
