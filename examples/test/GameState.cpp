#include "SkellEngine/states/State.h"
#include "SkellEngine/systems/SystemManager.h"
#include "SkellEngine/levels/Level.h"
#include "SkellEngine/states/StateManager.h"
#include "SkellEngine/entities/EntityManager.h"
#include "SkellEngine/files/FileManager.h"

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

        tgui::Theme blackTheme{getStateContext().context.fileManager->getGameDescription().media.baseMediaPath / "Black.txt"};
        tgui::Theme::setDefault(&blackTheme);

        auto button = tgui::Button::create("BUTTON");
        button->setPosition(100, 200);
        getStateContext().gui->add(button);
    }

    virtual void onDestroy() override {
    }

    virtual void activate() override {
        auto eventHandler = getStateContext().context.eventHandler;
        auto player = getStateContext().entityManager->getEntity("player");
        eventHandler->sendEvent("PlayAnimation", {
            {"animation", {"animation", "framing"}}
        }, {player});
        eventHandler->sendEvent("PlayAnimation", {{"animation", "fading"}, {"queue", 1}}, {player});
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