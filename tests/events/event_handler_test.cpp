#include <catch.hpp>

#include "SkellEngine/events/Event.hpp"
#include "SkellEngine/events/EventHandler.hpp"

#include "SkellEngine/Context.hpp"

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

    REGISTER_EVENT(TestEvent, "test")
};

bool called1 = false;
bool called2 = false;

void testCallback1(const events::Event &event) {
    called1 = !called1;
    REQUIRE(event.get<long>("integer").second);
    REQUIRE(event.get<long>("integer").first == 10);
}

void testCallback2(const events::Event &event) {
    called2 = !called2;
}

void unknownCallback(const events::Event &event) {

}

SCENARIO("Event handler accepts new connections, disconnections and can send events", "[events]") {
    Context context("@CMAKE_CURRENT_SOURCE_DIR@/media", false);

    events::EventHandler handler(context);

    events::Event::Ptr event = events::Event::createEvent("test", context);
    event->set("integer", 10);

    called1 = called2 = false;

    GIVEN("A known event type") {
        auto connect1 = handler.registerCallback("test", &testCallback1);
        auto connect2 = handler.registerCallback("test", &testCallback2);
        REQUIRE(connect1.isConnected());
        REQUIRE(connect2.isConnected());

        THEN ("The callbacks are called") {
            REQUIRE_FALSE(called1);
            REQUIRE_FALSE(called2);

            REQUIRE(handler.sendEvent(*event));

            REQUIRE(called1);
            REQUIRE(called2);
        }

        WHEN ("We copy a connection") {
            auto connect1bis = connect1;

            REQUIRE(connect1.isConnected());
            REQUIRE(connect1bis.isConnected());

            AND_WHEN("We disconnect the original") {
                connect1.disconnect();

                THEN ("The original and the copy are disconnected") {
                    REQUIRE(!connect1.isConnected());
                    REQUIRE(!connect1bis.isConnected());
                }
            }

            AND_WHEN("We disconnect the copy") {
                connect1bis.disconnect();

                THEN ("The original and the copy are disconnected") {
                    REQUIRE(!connect1.isConnected());
                    REQUIRE(!connect1bis.isConnected());
                }
            }
        }

        WHEN ("We remove the first callback") {
            connect1.disconnect();
            REQUIRE(!connect1.isConnected());
            REQUIRE(connect2.isConnected());

            THEN ("The second callback is still active") {
                REQUIRE(handler.sendEvent(*event));

                REQUIRE_FALSE(called1);
                REQUIRE(called2);
            }
        }

        WHEN ("We remove the second callback") {
            connect2.disconnect();
            REQUIRE(connect1.isConnected());
            REQUIRE(!connect2.isConnected());

            THEN ("The first callback is still active") {
                REQUIRE(handler.sendEvent(*event));

                REQUIRE(called1);
                REQUIRE_FALSE(called2);
            }
        }

        WHEN("We remove every callback") {
            connect1.disconnect();
            connect2.disconnect();
            REQUIRE(!connect1.isConnected());
            REQUIRE(!connect2.isConnected());

            THEN("The callbacks are not called") {
                REQUIRE_FALSE(handler.sendEvent(*event));

                REQUIRE_FALSE(called1);
                REQUIRE_FALSE(called2);
            }
        }

        WHEN("We clear the handler") {
            handler.clear();

            REQUIRE_FALSE(handler.sendEvent(*event));

            REQUIRE_FALSE(called1);
            REQUIRE_FALSE(called2);
        }
    }

    GIVEN("An unknown event type") {
        auto connect3 = handler.registerCallback("unknown", &unknownCallback);
        REQUIRE(connect3.isConnected());

        REQUIRE_FALSE(called1);
        REQUIRE_FALSE(called2);

        REQUIRE_FALSE(handler.sendEvent(*event));

        REQUIRE_FALSE(called1);
        REQUIRE_FALSE(called2);
    }
}