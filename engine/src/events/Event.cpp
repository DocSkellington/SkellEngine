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

    void Event::luaFunctions(sol::state &lua) {
        lua.new_usertype<Event>("event",
            "getType", &Event::getType,
            sol::base_classes, sol::bases<utilities::MemberStorage>()
        );
    }
}