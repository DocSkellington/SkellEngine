#include "entities/components/Component.h"

#include "entities/components/ExternComponent.h"

namespace engine::entities::components {
    Component::Component() {

    }

    Component::~Component() {

    }

    Component::Ptr Component::createInstance(const std::string &componentType) {
        auto compoConstructor = getMapToComponent()->find(componentType);
        if (compoConstructor == getMapToComponent()->end())
            return std::make_shared<ExternComponent>();
        return compoConstructor->second();
    }

    std::shared_ptr<Component::MapType> Component::getMapToComponent() {
        static std::shared_ptr<Component::MapType> map = std::make_shared<Component::MapType>();
        return map;
    }
}