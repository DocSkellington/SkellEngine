#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

namespace engine::states {
    class StateManager;

    /**
    * \brief Defines the base of every state
    * \ingroup Engine
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
        virtual void update(sf::Int64 deltatime) = 0;
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

        /**
         * \brief If the state is transcendant, the events can be processed by the following state (in the stack)
         * \return True iff the state is transcendant
         */
        bool isTranscendant() const;
        /**
         * \brief If the state is transparent, the following state (in the stack) can be drawn
         * \return True iff the state is transparent
         */
        bool isTransparent() const;

    protected:
        StateManager& getStateManager() const;
        sf::View getView() const;
        void setView(const sf::View &view);

    private:
        bool m_transcendant;
        bool m_transparent;
        StateManager &m_stateManager;

        sf::View m_view;
    };
}