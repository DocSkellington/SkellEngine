/**
 * \file State.h
 */

#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "SkellEngine/utilities/RegisterClass.h"
#include "SkellEngine/states/StateContext.h"
#include "SkellEngine/events/StoreEventConnections.h"

namespace engine::states {
    class StateManager;

    /**
    * \brief Defines the base of every state
    * \see REGISTER_STATE for a macro to register a new state
    */
    class State {
    public:
        /**
         * \brief Every state should be manipulated through this pointer type
         */
        using Ptr = std::shared_ptr<State>;

    public:
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
         * \brief Handles an sf::Event
         * 
         * The default implementation passes the event to the GUI
         * \param event The event
         */
        virtual void handleEvent(const sf::Event &event);
        /**
        * \brief Draws the state
        * \param target The target in which the rendering must be done
        */
        virtual void draw(sf::RenderTarget &target) = 0;

        /**
         * \brief If the state is transcendant, the following state (in the stack) can be updated
         * \return True iff the state is transcendant
         */
        bool isTranscendant() const;
        /**
         * \brief If the state is transparent, the following state (in the stack) can be drawn
         * \return True iff the state is transparent
         */
        bool isTransparent() const;

        /**
         * \brief Sets if the state if transcendant
         * 
         * If the state is transcendant, it means that the next state in the stack can also be updated
         * \param isTranscendant Whether the state is transcendant
         */
        void setIsTranscendant(bool isTranscendant);

        /**
         * \brief Sets if the state if transparent
         * 
         * If the state is transparent, it means that the next state in the stack can also be drawn
         * \param isTransparent Whether the state is transparent
         */
        void setIsTransparent(bool isTransparent);

        /**
         * \brief Creates a state of the given name
         * 
         * The state must be registered
         * \param name The name of the state
         * \param manager The state manager
         * \return The state
         * \throws Exceptions are thrown if the system could not be created 
         */
        static Ptr createInstance(const std::string &name, StateManager &manager);

    protected:
        /**
         * \brief The specialisation of utilities::RegisterClass for the states
         */
        using RegisteredStates = utilities::RegisterClass<State, StateManager&, const std::string&>;

        /**
         * \brief A shortcut to register a state
         * \tparam T The type of the state to register
         */
        template <typename T>
        using RegisterState = RegisteredStates::Register<T>;

    protected:
        /**
         * \brief The constructor
         * \param manager The manager
         * \param stateName The name of the state
         */
        State(StateManager &manager, const std::string &stateName);

        /**
         * \brief Constructor
         * \param manager The state manager
         * \param stateName The name of the state
         * \param isTranscendant Whether the state is transcendant
         * \param isTransparent Whether the state is transparent
         */
        State(StateManager &manager, const std::string &stateName, bool isTranscendant, bool isTransparent);

        /** @{ */
        /**
         * \brief Gives the state context
         * \return The state context
         */
        StateContext& getStateContext();
        const StateContext& getStateContext() const;
        /** @} */

        /** @{ */
        /**
         * \brief Gives the storage of event connections
         * \return The storage
         */
        events::StoreEventConnections& getStoreEventConnections();
        const events::StoreEventConnections& getStoreEventConnections() const;
        /** @} */

        /**
         * \brief Registers a new callback in the storage for this system
         * 
         * The connection is automatically removed when the system dies
         * \param eventType The type of the event to listen to
         * \param callback The callback to add
         * \param state The state of the game in which the callback must be active
         * \return The connection to the registered callback
         */
        events::EventConnection registerCallback(const std::string &eventType, const events::EventHandler::callbackSignature &callback, const std::string &state = "all");

    private:
        bool m_transcendant;
        bool m_transparent;

        StateContext m_stateContext;

        events::StoreEventConnections m_storeEventConnections;
    };
}

/**
 * \brief Registers the state TYPE under the name NAME
 * 
 * \warning It must be placed <b>inside</b> of the class definition. For example, do something like:
 * \code
 * class ExampleState : public State {
 *  public:
 *      ExampleState(StateManager& manager) : State(manager) { ... }
 *      ...
 *      REGISTER_STATE(ExampleState, "example")
 * };
 * \endcode
 * 
 * \note This macro adds a private member variable. The name of the variable is the concatenation of "registeringVariable" and the line number in the header file using this macro. This allows to register multiple times the same component under different names
 * \warning This macro uses "private: ". Therefore, everything declared after this macro will be marked as private in your class definition.
 */
#define REGISTER_STATE(TYPE, NAME) REGISTER_CLASS(RegisterState, TYPE, NAME)
