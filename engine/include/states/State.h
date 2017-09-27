#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

namespace engine::states {
    class StateManager;

    /**
    * \brief Defines the base of every state
    * \todo all
    */
    class State {
    public:
        explicit State(StateManager &manager);
        State(const State&) = delete;
        virtual ~State();

        /**
        * \brief Performs actions on the creation of the state.
        */
        virtual void onCreate() = 0;
        /**
        * \brief Performs actions on the destruction of the state.
        */
        virtual void onDestroy() = 0;

        /**
        * \brief Activates the state
        */
        virtual void activate() = 0;
        /**
        * \brief Deactivates the state
        */
        virtual void deactivate() = 0;

        /**
        * \brief Updates the state
        * \param deltatime The time since the last update
        */
        virtual void update(float deltatime) = 0;
        /**
        * \brief Handles an event
        * \param event The event to process
        */
        virtual void handleEvent(sf::Event &event) = 0;
        /**
        * \brief Draws the state
        * \param window The window in which the rendering must be done
        */
        virtual void draw(std::shared_ptr<sf::RenderWindow> &window) = 0;

        bool isTranscendant() const;
        bool isTransparent() const;

    protected:
        StateManager& getStateManager() const;

    private:
        bool m_transcendant;
        bool m_transparent;
        StateManager &m_stateManager;
    };
}