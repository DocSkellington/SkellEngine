#pragma once

#include "entities/components/Component.h"
#include "Context.h"

namespace engine::entities::components {
    /**
     * \brief This component is defined only through JSON and Lua scripts.
     * 
     * Its purpose is to be used in systems created by the user.
     */
    class ExternComponent : public Component {
    public:
        ExternComponent();
        ExternComponent(const ExternComponent&) = delete;
        virtual ~ExternComponent();

        void create(Context &context, const nlohmann::json &jsonTable) override;

    private:
        nlohmann::json jsonTable;
    };
}