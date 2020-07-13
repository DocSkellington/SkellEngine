#include <catch.hpp>
#include <nlohmann/json.hpp>

#include "SkellEngine/entities/Entity.hpp"
#include "SkellEngine/Context.hpp"
#include "SkellEngine/files/FileManager.hpp"

using namespace engine;
using namespace entities;

SCENARIO("Entity test", "[entities]") {
    Context context("@CMAKE_CURRENT_SOURCE_DIR@/media", false);
    states::StateContext stateContext(context, "falseState");
    
    Entity entity(*stateContext.entityManager, "dummy");

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
                    REQUIRE(pos->getDouble("x").second);
                    REQUIRE(pos->getDouble("x").first == 0);
                    REQUIRE(pos->getDouble("y").second);
                    REQUIRE(pos->getDouble("y").first == 0);
                    REQUIRE(pos->getVector2d("position").second);
                    REQUIRE(pos->getVector2d("position").first == Vector2d(0, 0));
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
            REQUIRE(pos->getDouble("x").second);
            REQUIRE(pos->getDouble("x").first == 19);
            REQUIRE(pos->getDouble("y").second);
            REQUIRE(pos->getDouble("y").first == 20);
            REQUIRE(pos->getVector2d("position").second);
            REQUIRE(pos->getVector2d("position").first == Vector2d(19, 20));
        }
    }
}