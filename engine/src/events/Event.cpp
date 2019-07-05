#include "SkellEngine/events/Event.h"

namespace engine::events {
    Event::Event(const std::string &type) :
        m_type(type) 
        {
        registerMember("type", &m_type);
    }

    Event::~Event() {

    }

    const std::string& Event::getType() const noexcept {
        return m_type;
    }

    std::shared_ptr<Event::mapType> Event::getMapToEvents() {
        static std::shared_ptr<mapType> map = std::make_shared<mapType>();
        return map;
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

    Event::Ptr Event::createEvent(const std::string &type) {
        auto constructor = getMapToEvents()->find(type);
        Event::Ptr ptr;

        if (constructor == getMapToEvents()->end()) {
            // TODO: ExternalEvent (once Lua interface with systems is done)
            ptr = nullptr;
        }
        else {
            ptr = constructor->second();
        }

        return ptr;
    }
}