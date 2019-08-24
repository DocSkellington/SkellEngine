#include "SkellEngine/events/ExternEvent.h"

namespace engine::events {
    ExternEvent::ExternEvent(const std::string &type, Context &context) :
        MemberStorage(context),
        Event(context, type),
        ExternMemberStorage(context)
        {

    }

    void ExternEvent::create(const nlohmann::json &jsonTable) {
        setInternalJSONTable(jsonTable);
    }

    void ExternEvent::luaFunctions(sol::state &lua) {
        lua.new_usertype<ExternEvent>("ExternEvent",
            sol::base_classes, sol::bases<utilities::ExternMemberStorage, events::Event>()
        );
    }

    std::string ExternEvent::getLogErrorPrefix() const {
        return "ExternEvent";
    }
}