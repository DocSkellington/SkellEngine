#include "SkellEngine/systems/System.hpp"
#include "SkellEngine/systems/SystemManager.hpp"
#include "SkellEngine/Context.hpp"

using namespace engine::systems;

class TestSystem : public System {
public:
    TestSystem(SystemManager &manager) :
        System(manager) {
        registerCallback("test", std::bind(&TestSystem::callbackTest, this, std::placeholders::_1));
        manager.getContext().context.eventHandler->sendEvent("test", { {"integer", 10}, {"floating", 1.5} });
    }
    ~TestSystem() {
        getStoreEventConnections().clearEventConnections();
    }

    virtual bool update(sf::Int64 deltatime, sf::View &view) override {
        return true;
    }

protected:
    virtual bool checkComponents(engine::entities::Entity::Ptr entity) const override {
        return false;
    }

    void callbackTest(const engine::events::Event &event) {
        std::cout << event.get<long>("integer").first << "\n";
    }

    REGISTER_SYSTEM(TestSystem, "testcpp")
};