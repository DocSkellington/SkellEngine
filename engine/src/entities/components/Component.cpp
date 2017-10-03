#include "entities/components/Component.h"

#include "entities/components/ExternComponent.h"

namespace engine::entities::components {
    Component::MapType Component::m_nameToComponent;

    Component::Component() {

    }

    Component::~Component() {

    }

    Component::Ptr Component::createInstance(const std::string &componentType) {
        auto compoConstructor = getMapToComponent().find(componentType);
        if (compoConstructor == getMapToComponent().end())
            return std::make_shared<ExternComponent>();
        return compoConstructor->second();
    }

    Component::MapType& Component::getMapToComponent() {
        return Component::m_nameToComponent;
    }
}