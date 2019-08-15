#include <string>
#include <memory>

#include "catch.hpp"
#include "SkellEngine/systems/System.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/Context.h"
#include "SkellEngine/entities/Entity.h"

using namespace engine;
using namespace systems;
using namespace entities;

class TestSystem : public System {
public:
    typedef std::shared_ptr<TestSystem> Ptr;

public:
    int nEntities = 0;
    sf::Int64 updateTime = sf::Int64(0);

public:
    TestSystem(SystemManager& manager) : System(manager) {

    }
    ~TestSystem() {

    }

    bool update(sf::Int64 deltaTime, sf::View &view) override {
        updateTime = deltaTime;
        return true;
    }

    bool addEntity(Entity::Ptr entity) override {
        bool a = System::addEntity(entity);
        if (a) {
            nEntities++;
        }
        return a;
    }

    bool removeEntity(Entity::Ptr entity) override {
        bool a = System::removeEntity(entity);
        if (a) {
            nEntities--;
        }
        return a;
    }

protected:
    bool checkComponents(Entity::Ptr entity) const override {
        return entity->hasComponent("testComponent");
    }

private:
    static System::RegisterSystem<TestSystem> rsts;
};

System::RegisterSystem<TestSystem> TestSystem::rsts("test");

SCENARIO("System manager functions", "[systems]") {
    Context context("@CMAKE_CURRENT_SOURCE_DIR@/media", false);
    states::StateContext stateContext(context, "falseState");
    SystemManager &sysMan = *stateContext.systemManager;

    GIVEN("A test system") {
        REQUIRE(sysMan.addSystem("Test"));

        WHEN("We add again the same system") {
            REQUIRE_FALSE(sysMan.addSystem("test"));
        }

        TestSystem::Ptr sys = std::static_pointer_cast<TestSystem>( sysMan.getSystem("test"));

        REQUIRE(sys);

        WHEN("We update the system manager") {
            sysMan.update(sf::Int64(100));
            THEN ("The update time of the test system is changed") {
                REQUIRE(sys->updateTime == 100);

                AND_WHEN ("We update again the system manager") {
                    sysMan.update(50);
                    THEN ("The update time of the test system is different") {
                        REQUIRE(sys->updateTime == 50);
                    }
                }
            }
        }

        WHEN ("We add an entity to the manager") {
            Entity::Ptr ent = std::make_shared<Entity>(*stateContext.entityManager, "testType");

            REQUIRE(sys->nEntities == 0);

            AND_WHEN ("The entity has a test component") {
                // We create an empty JSON to create the component
                nlohmann::json jTable;
                ent->addComponent("testComponent", jTable);

                REQUIRE(sysMan.addEntity(ent));

                THEN ("The test system has changed") {
                    REQUIRE(sys->nEntities == 1);

                    AND_THEN ("The entity can be removed") {
                        REQUIRE(sysMan.removeEntity(ent));

                        AND_THEN ("The test system has changed") {
                            REQUIRE(sys->nEntities == 0);
                        }
                    }
                }
            }

            AND_WHEN ("The entity does not have any components") {
                REQUIRE_FALSE(sysMan.addEntity(ent));

                THEN ("The test system has not changed") {
                    REQUIRE(sys->nEntities == 0);

                    AND_THEN ("The entity can not be removed") {
                        REQUIRE_FALSE(sysMan.removeEntity(ent));

                        AND_THEN("The test system still has not changed") {
                            REQUIRE(sys->nEntities == 0);
                        }
                    }
                }
            }
        }

        WHEN ("We remove the test system from the manager") {
            REQUIRE(sysMan.removeSystem("test"));
            THEN ("The test system is no longer found in the system") {
                REQUIRE_FALSE(sysMan.getSystem("test"));

                AND_THEN("The test system still exists") {
                    REQUIRE(sys);
                }
            }
        }

        WHEN ("We clear the system manager") {
            sysMan.clear();
            THEN ("The test system is removed but still exists") {
                REQUIRE_FALSE(sysMan.getSystem("test"));
                REQUIRE(sys);
            }
        }
    }
}