#include "SkellEngine/events/ExternEvent.hpp"

namespace engine::events {
    ExternEvent::ExternEvent(const std::string &type, Context &context) :
        Event(context, type) {

    }

    void ExternEvent::create(const nlohmann::json &jsonTable) {
        loadFromJSON(jsonTable);
    }

    void ExternEvent::luaFunctions(sol::state &lua) {
        lua.new_usertype<ExternEvent>("ExternEvent",
            sol::base_classes, sol::bases<utilities::VariableStorage, events::Event>()
        );
    }

    std::string ExternEvent::getLogErrorPrefix() const {
        return "ExternEvent";
    }
}