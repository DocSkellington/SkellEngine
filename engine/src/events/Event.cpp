#include "SkellEngine/events/Event.h"

#include "SkellEngine/Context.h"
#include "SkellEngine/events/ExternEvent.h"

namespace engine::events {
    Event::Event(Context &context, const std::string &type) :
        MemberStorage(context),
        m_type(type)
        {
        registerMember("type", &m_type);
    }

    Event::~Event() {

    }

    const std::string& Event::getType() const noexcept {
        return m_type;
    }

    std::string Event::getLogErrorPrefix() const {
        return "Event";
    }

    void Event::luaFunctions(sol::state &lua) {
        lua.new_usertype<Event>("event",
            "getType", &Event::getType,
            sol::base_classes, sol::bases<utilities::MemberStorage>()
        );
    }

    Event::Ptr Event::createEvent(const std::string &type, Context &context) {
        Event::Ptr ptr = RegisteredEvents::construct(type, context);

        if (!ptr) {
            tmx::Logger::log("Event: createEvent: creating an ExternEvent since " + type + " is not registered");
            ptr = std::make_shared<ExternEvent>(type, context);
        }

        return ptr;
    }

    Event::Ptr Event::createEvent(const std::string &type, Context &context, const nlohmann::json &jsonTable) {
        Event::Ptr event = createEvent(type, context);
        event->create(jsonTable);
        return event;
    }
}