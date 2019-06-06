#include <catch.hpp>
#include <nlohmann/json.hpp>

#include "SkellEngine/entities/Entity.h"
#include "SkellEngine/Context.h"

using namespace engine;
using namespace entities;

SCENARIO("Entity test", "[entities]") {
    Context context;
    
    Entity entity(context);

    WHEN("The entity is empty") {
        THEN("The component does not have any component") {
            REQUIRE(entity.size() == 0);
            REQUIRE_FALSE(entity.hasComponent("position"));
        }

        AND_WHEN("We can add a default 'position' component") {
            REQUIRE(entity.addComponent("position"));

            THEN("The component is no longer empty") {
                REQUIRE(entity.hasComponent("position"));
                REQUIRE(entity.size() == 1);
            }

            THEN("We can retrieve the component") {
                auto pos = entity.getComponent("position");
                REQUIRE(pos);

                AND_THEN("The component is a PositionComponent") {
                    REQUIRE(pos->getFloat("x").second);
                    REQUIRE(pos->getFloat("x").first == 0);
                    REQUIRE(pos->getFloat("y").second);
                    REQUIRE(pos->getFloat("y").first == 0);
                }
            }

            THEN("We cannot add a new 'position' component") {
                REQUIRE_FALSE(entity.addComponent("position"));
            }

            THEN("We can iterator over the entity") {
                for (auto &itr : entity) {
                    REQUIRE(itr.first == "position");
                }
            }
        }

        AND_WHEN("We can add a 'position' component with a JSON table") {
            REQUIRE(entity.addComponent("position", {{"x", 19}, {"y", 20}}));

            auto pos = entity.getComponent("position");
            REQUIRE(pos);
            REQUIRE(pos->getFloat("x").second);
            REQUIRE(pos->getFloat("x").first == 19);
            REQUIRE(pos->getFloat("y").second);
            REQUIRE(pos->getFloat("y").first == 20);
        }
    }
}