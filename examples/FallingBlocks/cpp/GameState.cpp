#include "SkellEngine/states/State.hpp"
#include "SkellEngine/states/StateManager.hpp"
#include "SkellEngine/systems/SystemManager.hpp"
#include "SkellEngine/levels/Level.hpp"
#include "SkellEngine/input/InputHandler.hpp"
#include "SkellEngine/entities/EntityManager.hpp"

#include "Grid.hpp"

class GameState : public engine::states::State {
public:
    GameState(engine::states::StateManager &manager, const std::string &name) :
        State(manager, name, false, false)
        {
    }

    virtual void onCreate() override {
        // We register a callback to the 'WindowClosed'
        registerCallback("WindowClosed", [&](const engine::events::Event &event) { getStateContext().context.closeEngine(); });

        // We load the system we need:
        //     GraphicalOrthogonal is our drawing system
        getStateContext().systemManager->loadSystems({"graphicalorthogonal"});

        registerCallback("Drop", [&](const engine::events::Event &event) {
            if (!m_grid) {
                m_grid = std::make_unique<Grid>(m_gridSize.x, m_gridSize.y, getStateContext());
                m_timeSinceLastMove = 0;
            }
        });
        registerCallback("GameOver", [&](const engine::events::Event &event) {
            std::cout << "Game over. Score: " << event.getInt("score").first << "\n";
            m_grid = nullptr;
        });

        // TODO: draw outlines of the game area
        // TODO: create a proper GUI library
    }

    virtual void onDestroy() override {
    }

    virtual void activate() override {
    }

    virtual void deactivate() override {
    }

    virtual void update(sf::Int64 deltaTime) override {
        m_timeSinceLastMove += deltaTime;
        if (m_grid && m_timeSinceLastMove >= m_timeBetweenTwoMoves) {
            m_timeSinceLastMove = 0;
            m_grid->moveMainDown();
        }

        // We update the graphical system
        getStateContext().systemManager->update(deltaTime);
    }

    virtual void draw(sf::RenderTarget &target) override {
        getStateContext().systemManager->draw(target, 0);
    }

private:
    std::unique_ptr<Grid> m_grid;
    const sf::Vector2u m_gridSize = {10, 20};
    sf::Int64 m_timeSinceLastMove = 0;
    sf::Int64 m_timeBetweenTwoMoves = 1000000; // One second

private:
    REGISTER_STATE(GameState, "game")
};