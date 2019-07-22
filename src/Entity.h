//
// Created by Ido Mozes on 20/06/2019.
//

#ifndef SDL2_GAME_ENTITY_H
#define SDL2_GAME_ENTITY_H

#include <bitset>
#include <vector>
#include <array>
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
    std::array<std::unique_ptr<Component>,ComponentType::LENGTH> components;
public:
    template<class ... T>
    std::optional<std::tuple<T &...>> getComponents() {
        constexpr uint64_t mask = createMask({T::type...});
        if ((componentsMask.to_ullong() & mask) == mask) {
            return std::tuple<T &...>((*(T *) components[T::type].get())...);
        }
        return std::nullopt;
    }

    template<class ... T>
    std::tuple<T *...> getComponentsP() {
        return std::tuple<T *...>(((T *) components[T::type].get())...);
    }

    template<class T>
    T *getComponent() {
        if (auto &c = components[T::type])
            return (T *) c.get();
        return nullptr;
    }

    template<typename ... Targs>
    void addComponents(Targs &... args) {
        std::initializer_list<std::tuple<ComponentType, Component *>> newComponents{
                std::make_tuple(Targs::type, new Targs(args))...};
        for (auto[t, p] :newComponents) {
            componentsMask[t] = true;
            components[t] = std::move(std::unique_ptr<Component>(p));
        }
    }

    template<typename T, typename ... Targs>
    T &addComponent(Targs &&... args) {
        T *c = new T(std::forward<Targs>(args)...);
        components[T::type] = std::move(std::unique_ptr<Component>(c));
        componentsMask[T::type] = true;
        return *c;
    }

    template<typename T>
    void removeComponent() {
        components[T::type].reset(nullptr);
        componentsMask[T::type] = false;
    }

};

#endif //SDL2_GAME_ENTITY_H
