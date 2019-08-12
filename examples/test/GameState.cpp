#include "SkellEngine/states/State.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/levels/Level.h"
#include "SkellEngine/states/StateManager.h"

class GameState : public engine::states::State {
public:
    /**
     * \brief The constructor
     * \param manager The state manager
     */
    GameState(engine::states::StateManager& manager) :
        State(manager, false, false) {

    }
    virtual ~GameState() {

    }

    void onChangeState(const engine::events::Event &) {
        getStateContext().context.stateManager->switchTo("mainmenu");
    }

    virtual void onCreate() override {
        getStateContext().systemManager->loadSystems({"graphicalorthogonal", "test", "testcpp"});
        registerCallback("ChangeState", std::bind(&GameState::onChangeState, this, std::placeholders::_1), "game");
        getStateContext().level->changeLevel("tutorial");
    }

    virtual void onDestroy() override {
        // getStateContext().context.gui->removeAllWidgets();
    }

    virtual void activate() override {
    }

    virtual void deactivate() override {
        
    }

    virtual void update(sf::Int64 deltatime) override {
        getStateContext().level->update(deltatime);
        getStateContext().systemManager->update(deltatime);
    }

    virtual void draw(sf::RenderWindow &window) override {
        getStateContext().level->draw(window);
    }

    REGISTER_STATE(GameState, "game")
};