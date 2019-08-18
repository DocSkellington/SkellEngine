#include "SkellEngine/states/State.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/levels/Level.h"
#include "SkellEngine/states/StateManager.h"
#include "SkellEngine/entities/EntityManager.h"

class GameState : public engine::states::State {
public:
    /**
     * \brief The constructor
     * \param manager The state manager
     */
    GameState(engine::states::StateManager& manager, const std::string &stateName) :
        State(manager, stateName, false, false) {

    }
    virtual ~GameState() {

    }

    void onChangeState(const engine::events::Event &) {
        getStateContext().context.stateManager->switchTo("mainmenu");
    }

    virtual void onCreate() override {
        getStateContext().systemManager->loadSystems({"graphicalorthogonal", "animation", "testcpp", "test"});
        registerCallback("ChangeState", std::bind(&GameState::onChangeState, this, std::placeholders::_1), "game");
        getStateContext().level->changeLevel("tutorial");
    }

    virtual void onDestroy() override {
        // getStateContext().context.gui->removeAllWidgets();
    }

    virtual void activate() override {
        getStateContext().context.eventHandler->sendEvent("PlayAnimation", {{"animation", "coloring"}}, {getStateContext().entityManager->getEntity("player")});
    }

    virtual void deactivate() override {
        
    }

    virtual void update(sf::Int64 deltatime) override {
        getStateContext().level->update(deltatime);
        getStateContext().systemManager->update(deltatime);
    }

    virtual void draw(sf::RenderTarget &target) override {
        getStateContext().level->draw(target);
        getStateContext().gui->draw();
    }

    REGISTER_STATE(GameState, "game")
};