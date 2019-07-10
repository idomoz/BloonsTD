#include "../Component.h"

class __: public Component{
public:
    static constexpr ComponentType type = ComponentType::__;
    __(Entity *entity):Component(entity){}
    ~__() override = default;
};