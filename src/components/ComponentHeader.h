#include "../Component.h"

class __: public Component{
public:
    static ComponentType getComponentType() { return ComponentType::__; }
    __(Entity *entity):Component(entity){}
};