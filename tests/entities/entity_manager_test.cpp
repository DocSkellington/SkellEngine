#include <catch.hpp>

#include "entities/EntityManager.h"
#include "Context.h"
#include "systems/SystemManager.h"

using namespace engine;
using namespace entities;

SCENARIO("Entity Manager", "[entities]") {
    Context context;
    context.systemManager = std::make_shared<systems::SystemManager>(context);
    auto entMan = context.entityManager = std::make_shared<EntityManager>(context);

    GIVEN("An empty manager") {
        REQUIRE(entMan->getNumberOfEntities() == 0);
        REQUIRE_FALSE(entMan->getEntity("entity"));

        THEN("We cannot remove anything") {
            Entity::Ptr entity = std::make_shared<Entity>(context, "entity");
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
    }
}