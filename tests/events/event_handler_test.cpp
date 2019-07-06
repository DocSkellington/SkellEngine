#include <catch.hpp>

#include "SkellEngine/events/Event.h"
#include "SkellEngine/events/EventHandler.h"

#include "SkellEngine/Context.h"

using namespace engine;

class TestEvent : public events::Event {
public:
    TestEvent(Context &context) :
        events::Event(context, "test")
        {
        registerMember("integer", &integer);
    }

    void create(const nlohmann::json &jsonTable) override {

    }

    int integer = 0;

    static events::Event::RegisterEvent<TestEvent> rete;
};
events::Event::RegisterEvent<TestEvent> TestEvent::rete = events::Event::RegisterEvent<TestEvent>("test");

bool called1 = false;
bool called2 = false;

void testCallback1(const events::Event &event) {
    called1 = !called2;
    REQUIRE(event.getInt("integer").second);
    REQUIRE(event.getInt("integer").first == 10);
}

void testCallback2(const events::Event &event) {
    called2 = !called2;
}

void unknownCallback(const events::Event &event) {

}

SCENARIO("Event handler", "[events]") {
    Context context;

    events::EventHandler handler(context);

    events::Event::Ptr event = events::Event::createEvent("test", context);
    event->set("integer", 10);

    called1 = called2 = false;

    GIVEN("A known event type") {
        int ID1 = handler.registerCallback("test", &testCallback1);
        int ID2 = handler.registerCallback("test", &testCallback2);
        REQUIRE(ID1 != -1);
        REQUIRE(ID2 != -1);
        REQUIRE(ID1 != ID2);

        REQUIRE_FALSE(called1);
        REQUIRE_FALSE(called2);

        REQUIRE(handler.sendEvent(*event));

        REQUIRE(called1);
        REQUIRE(called2);

        WHEN("We remove the event giving the correct type") {
            REQUIRE(handler.removeCallback("test", ID1));
            REQUIRE(handler.removeCallback("test", ID2));

            REQUIRE_FALSE(handler.sendEvent(*event));

            REQUIRE(called1);
            REQUIRE(called2);
        }

        WHEN("We remove the event giving the wrong type") {
            REQUIRE_FALSE(handler.removeCallback("unknown", ID1));
            REQUIRE_FALSE(handler.removeCallback("unknown", ID2));

            REQUIRE(handler.sendEvent(*event)); // There are still two callbacks

            REQUIRE_FALSE(called1);
            REQUIRE_FALSE(called2);
        }

        WHEN("We clear the handler") {
            handler.clear();

            REQUIRE_FALSE(handler.sendEvent(*event));

            REQUIRE(called1);
            REQUIRE(called2);
        }
    }

    GIVEN("An unknown event type") {
        REQUIRE(handler.registerCallback("unknown", &unknownCallback) != -1);

        REQUIRE_FALSE(called1);
        REQUIRE_FALSE(called2);

        REQUIRE_FALSE(handler.sendEvent(*event));

        REQUIRE_FALSE(called1);
        REQUIRE_FALSE(called2);
    }
}