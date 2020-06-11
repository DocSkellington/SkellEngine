#include <catch.hpp>

#include "SkellEngine/entities/EntityManager.hpp"
#include "SkellEngine/Context.hpp"
#include "SkellEngine/systems/SystemManager.hpp"

using namespace engine;
using namespace entities;

SCENARIO("Entity Manager", "[entities]") {
    Context context("@CMAKE_CURRENT_SOURCE_DIR@/media", false);
    states::StateContext stateContext(context, "falseState");
    auto entMan = stateContext.entityManager;

    GIVEN("An empty manager") {
        REQUIRE(entMan->getNumberOfEntities() == 0);
        REQUIRE_FALSE(entMan->getEntity("entity"));

        THEN("We cannot remove anything") {
            Entity::Ptr entity = std::make_shared<Entity>(*stateContext.entityManager, "entity");
            REQUIRE_FALSE(entMan->removeEntity(entity));
        }

        WHEN("We add an empty entity") {
            auto entity = entMan->addEntity("entity");
            REQUIRE(entity);

            REQUIRE(entMan->getNumberOfEntities() == 1);

            REQUIRE(entMan->getEntity("entity") == entity);

            AND_WHEN("We add a second entity") {
                REQUIRE(entMan->addEntity("entity"));
                REQUIRE(entMan->getNumberOfEntities() == 2);
                REQUIRE(entMan->getEntity("entity") == entity); // The first matching entity
            }

            AND_WHEN("We remove the entity") {
                REQUIRE(entMan->removeEntity(entity));
                REQUIRE(entMan->getNumberOfEntities() == 0);
                REQUIRE_FALSE(entMan->getEntity("entity"));
            }
        }

        WHEN("We add an entity with a position component") {
            // We only check that the entity is correctly constructed
            auto entity = entMan->addEntity("entity", {{"position", {4.1, 7}}});

            REQUIRE(entity);
            REQUIRE(entMan->getNumberOfEntities() == 1);

            auto position = entity->getComponent("position");
            REQUIRE(position);
            REQUIRE(position->getFloat("x").first == Approx(4.1));
            REQUIRE(position->getFloat("y").first == Approx(7));

            std::vector<std::string> components = {"position"};
            REQUIRE(entMan->getEntity("entity", {"position"}) == entity);
        }
    }
}