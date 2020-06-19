#pragma once

#include <vector>
#include <random>

#include "SkellEngine/entities/Entity.hpp"
#include "SkellEngine/states/StateContext.hpp"
#include "SkellEngine/events/StoreEventConnections.hpp"

// The grid has functions to convert global coordinates to grid coordinates and vice-versa (like a level)
// All computations are done with grid coordinates but positions are stored in global coordinates (to correctly draw)
// Handles movements of blocks
class Grid {
public:
    Grid(unsigned int width, unsigned int height, unsigned int cellSize, engine::states::StateContext &stateContext);
    ~Grid();

    bool moveMainDown();

private:
    enum BlockType {
        SQUARE = 0,
        LINE,
        T,
        L,
        REVERSED_L,
        Z,
        S
    };

private:
    void createNewMainBlock();

    bool moveMainRight();
    bool moveMainLeft();
    void dropMain();

    void rotateClockwise();

    bool canMoveDown(unsigned long x, unsigned long y) const;
    bool canMoveRight(unsigned long x, unsigned long y) const;
    bool canMoveLeft(unsigned long x, unsigned long y) const;

    std::list<unsigned int> findCompleteLines();

    sf::Vector2f fromGlobalToLocal(float x, float y) const;
    sf::Vector2f fromLocalToGlobal(sf::Vector2f localPosition) const;

private:
    engine::states::StateContext &m_context;
    // Line, column
    std::vector<std::vector<engine::entities::Entity::Ptr>> m_grid;

    const long m_cellSize;
    const sf::Vector2f m_origin;
    const long m_width;
    const long m_height;

    const sf::Vector2f m_topCenter;
    
    std::vector<engine::entities::Entity::Ptr> m_mainBlock;
    BlockType m_mainType;
    unsigned int m_mainRotation;

    std::default_random_engine m_rng;
    std::uniform_int_distribution<int> m_nextBlockGenerator;

    engine::events::StoreEventConnections m_eventConnections;
};