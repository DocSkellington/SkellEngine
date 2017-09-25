#include "states/State.h"
#include "states/StateManager.h"

namespace engine::states {
    /**
    * \brief Default implementation of the main menu state
    */
    class MainMenuState : public State {
    public:
        MainMenuState(StateManager& manager);
        virtual ~MainMenuState();

        virtual void onCreate();
        virtual void onDestroy();

        virtual void activate();
        virtual void deactivate();

        virtual void update(float deltatime);
        virtual void handleEvent(sf::Event &event);
        virtual void draw(std::shared_ptr<sf::RenderWindow> &window);
    };
}