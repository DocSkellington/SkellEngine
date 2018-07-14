#include "entities/components/ExternComponent.h"

namespace engine::entities::components {
    ExternComponent::ExternComponent() : Component() {

    }

    ExternComponent::~ExternComponent() {

    }

    void ExternComponent::create(Context &context, const nlohmann::json &jsonTable) {
        this->jsonTable = jsonTable;
    }
}