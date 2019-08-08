#include "SkellEngine/systems/System.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/Context.h"

using namespace engine::systems;

class TestSystem : public System {
public:
    TestSystem(SystemManager &manager) :
        System(manager) {
        registerCallback("test", std::bind(&TestSystem::callbackTest, this, std::placeholders::_1));
        manager.getContext().eventHandler->sendEvent("test", {{"integer", 10}});
    }

    virtual bool update(sf::Int64 deltatime, sf::View &view) override {
        return true;
    }

protected:
    virtual bool checkComponents(engine::entities::Entity::Ptr entity) const override {
        return false;
    }

    void callbackTest(const engine::events::Event &event) {
        std::cout << event.getInt("integer").first << "\n";
    }

    REGISTER_SYSTEM(TestSystem, "testcpp")
};