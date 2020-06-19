#include "Grid.hpp"

#include "SkellEngine/entities/EntityManager.hpp"
#include "SkellEngine/events/EventHandler.hpp"

// TODO: add a score system
// TODO: add animations, sounds, and so on

Grid::Grid(unsigned int width, unsigned int height, unsigned int cellSize, engine::states::StateContext &context) :
    m_context(context),
    m_cellSize(cellSize),
    m_origin(3 * m_cellSize, height * m_cellSize),
    m_width(width),
    m_height(height),
    m_nextBlockGenerator(0, 6),
    m_topCenter((width - 1) / 2, height - 1),
    m_eventConnections(*context.context.eventHandler) {
    m_grid.resize(height);
    for (auto &column : m_grid) {
        column.resize(width);
    }

    std::random_device rd;
    m_rng.seed(rd());

    m_mainBlock.resize(4);
    createNewMainBlock();

    m_eventConnections.registerCallback("MoveDown", [&](const engine::events::Event &event) { moveMainDown(); });
    m_eventConnections.registerCallback("MoveRight", [&](const engine::events::Event &event) { moveMainRight(); });
    m_eventConnections.registerCallback("MoveLeft", [&](const engine::events::Event &event) { moveMainLeft(); });
    m_eventConnections.registerCallback("Drop", [&](const engine::events::Event &event) { dropMain(); });
    m_eventConnections.registerCallback("RotateClockwise", [&](const engine::events::Event &event) { rotateClockwise(); });
}

Grid::~Grid() {
    for (auto &line : m_grid) {
        for (auto &block : line) {
            m_context.entityManager->removeEntity(block);
        }
    }
}

bool Grid::moveMainDown() {
    // First, we check if each part of the block can move down
    // If yes, then we can effectively move them down
    // If no, the block becomes fixed (and is added to the grid)
    bool shouldBecomeFixed = false;
    for (auto &block : m_mainBlock) {
        auto blockPosition = block->getComponent("position");
        double x = blockPosition->getFloat("x").first;
        double y = blockPosition->getFloat("y").first;
        sf::Vector2f localPosition = fromGlobalToLocal(x, y);

        if (!canMoveDown(localPosition.x, localPosition.y)) {
            shouldBecomeFixed = true;
            break;
        }
    }

    for (auto &block : m_mainBlock) {
        auto blockPosition = block->getComponent("position");
        double x = blockPosition->getFloat("x").first;
        double y = blockPosition->getFloat("y").first;
        sf::Vector2f localPosition = fromGlobalToLocal(x, y);

        if (shouldBecomeFixed) {
            m_grid[localPosition.y][localPosition.x] = block;
        }
        else {
            localPosition.y -= 1;
            blockPosition->set("y", fromLocalToGlobal(localPosition).y);
        }
    }

    if (shouldBecomeFixed) {
        // If the current main block became fixed, we check if a line is complete and we create a new block
        std::list<unsigned int> completeLines = findCompleteLines();

        // We empty the complete lines
        for (unsigned int line : completeLines) {
            for (unsigned int column = 0 ; column < m_width ; column++) {
                m_context.entityManager->removeEntity(m_grid[line][column]);
                m_grid[line][column] = nullptr;
            }
        }
        // We move down the above lines
        for (unsigned int line : completeLines) {
            for (unsigned int currentLine = line ; currentLine < m_height - 1 ; currentLine++) {
                unsigned int above = currentLine + 1;
                for (unsigned int column = 0 ; column < m_width ; column++) {
                    if (m_grid[above][column]) {
                        auto position = m_grid[above][column]->getComponent("position");
                        double x = position->getFloat("x").first;
                        double y = position->getFloat("y").first;
                        sf::Vector2f localPosition = fromGlobalToLocal(x, y);
                        localPosition.y -= 1;
                        sf::Vector2f globalPosition = fromLocalToGlobal(localPosition);
                        position->set("y", globalPosition.y);

                        m_grid[currentLine][column] = m_grid[above][column];
                        m_grid[above][column] = nullptr;
                    }
                }
            }
        }

        createNewMainBlock();
    }
    return !shouldBecomeFixed;
}

bool Grid::moveMainRight() {
    bool canMove = true;
    for (auto &block : m_mainBlock) {
        auto blockPosition = block->getComponent("position");
        double x = blockPosition->getFloat("x").first;
        double y = blockPosition->getFloat("y").first;
        std::cout << x << " " << y << "\n";
        sf::Vector2f localPosition = fromGlobalToLocal(x, y);
        std::cout << localPosition.x << " " << localPosition.y << "\n";
        if (!canMoveRight(localPosition.x, localPosition.y)) {
            canMove = false;
            break;
        }
    }

    if (canMove) {
        for (auto &block : m_mainBlock) {
            auto blockPosition = block->getComponent("position");
            double x = blockPosition->getFloat("x").first;
            double y = blockPosition->getFloat("y").first;
            sf::Vector2f localPosition = fromGlobalToLocal(x, y);
            localPosition.x += 1;
            blockPosition->set("x", fromLocalToGlobal(localPosition).x);
        }
    }

    return canMove;
}

bool Grid::moveMainLeft() {
    bool canMove = true;
    for (auto &block : m_mainBlock) {
        auto blockPosition = block->getComponent("position");
        double x = blockPosition->getFloat("x").first;
        double y = blockPosition->getFloat("y").first;
        sf::Vector2f localPosition = fromGlobalToLocal(x, y);
        if (!canMoveLeft(localPosition.x, localPosition.y)) {
            canMove = false;
            break;
        }
    }

    if (canMove) {
        for (auto &block : m_mainBlock) {
            auto blockPosition = block->getComponent("position");
            double x = blockPosition->getFloat("x").first;
            double y = blockPosition->getFloat("y").first;
            sf::Vector2f localPosition = fromGlobalToLocal(x, y);
            localPosition.x -= 1;
            blockPosition->set("x", fromLocalToGlobal(localPosition).x);
        }
    }

    return canMove;
}

void Grid::dropMain() {
    while (moveMainDown());
}

void Grid::rotateClockwise() {
    // First, we compute the positions of each part of the block after the rotation
    // Then, we check if we can effectively rotate the block (i.e., if each position is valid)
    sf::Vector2f positions[4];

    for (std::size_t i = 0 ; i < 4 ; i++) {
        auto position = m_mainBlock[i]->getComponent("position");
        double x = position->getFloat("x").first;
        double y = position->getFloat("y").first;
        positions[i] = fromGlobalToLocal(x, y);
    }

    if (m_mainType == BlockType::SQUARE) {
        // Nothing to rotate
    }
    else if (m_mainType == BlockType::LINE) {
        // We use the second part (so, index = 1) as the pivot
        if (m_mainRotation == 0 || m_mainRotation == 180) {
            // Horizontal line
            for (int i = 0 ; i < 4 ; i++) {
                positions[i].x = positions[1].x + (i - 1);
                positions[i].y = positions[1].y;
            }
        }
        else {
            // Vertical line
            for (int i = 0 ; i < 4 ; i++) {
                positions[i].x = positions[1].x;
                positions[i].y = positions[1].y + (i - 1);
            }
        }
    }
    else if (m_mainType == BlockType::T) {
        // The part that is in the middle of the base (index = 2) is the pivot
        for (int i = 0 ; i < 4 ; i++) {
            float xDiff = positions[2].x - positions[i].x;
            float yDiff = positions[2].y - positions[i].y;
            if (m_mainRotation == 0 || m_mainRotation == 180) {
                positions[i].x = positions[2].x - yDiff;
                positions[i].y = positions[2].y - xDiff;
            }
            else {
                positions[i].x = positions[2].x + yDiff;
                positions[i].y = positions[2].y + xDiff;
            }
        }
    }
    else if (m_mainType == BlockType::L || m_mainType == BlockType::REVERSED_L) {
        // Index = 1 is the pivot (the middle of the larger side)
        for (int i = 0 ; i < 3 ; i++) {
            float xDiff = positions[1].x - positions[i].x;
            float yDiff = positions[1].y - positions[i].y;
            if (m_mainRotation == 0 || m_mainRotation == 180) {
                positions[i].x = positions[1].x - yDiff;
                positions[i].y = positions[1].y - xDiff;
            }
            else {
                positions[i].x = positions[1].x + yDiff;
                positions[i].y = positions[1].y + xDiff;
            }
        }
        // We handle the last position explicitly
        int offset = (m_mainType == BlockType::L) ? -1 : 1;
        if (m_mainRotation == 0) {
            positions[3] = {positions[2].x, positions[2].y + offset};
        }
        else if (m_mainRotation == 90) {
            positions[3] = {positions[2].x + offset, positions[2].y};
        }
        else if (m_mainRotation == 180) {
            positions[3] = {positions[2].x, positions[2].y - offset};
        }
        else if (m_mainRotation == 270) {
            positions[3] = {positions[2].x - offset, positions[2].y};
        }
    }
    else if (m_mainType == BlockType::S || m_mainType == BlockType::Z) {
        // Index = 1 is the pivot
        // Since rotation = 0 and rotation = 180 both describe the exact same block, we can simplify the rotations
        int offset = (m_mainType == BlockType::S) ? -1 : 1;
        if (m_mainRotation == 0 || m_mainRotation == 180) {
            positions[0] = {positions[1].x, positions[1].y + offset};
            positions[2] = {positions[1].x - 1, positions[1].y};
            positions[3] = {positions[1].x - 1, positions[1].y - offset};
        }
        else {
            positions[0] = {positions[1].x - offset, positions[1].y};
            positions[2] = {positions[1].x, positions[1].y - 1};
            positions[3] = {positions[1].x + offset, positions[1].y - 1};
        }
    }

    for (const auto &position : positions) {
        if (0 > position.x || position.x > m_width - 1 || 0 > position.y || position.y > m_height - 1 || m_grid[position.y][position.x] != nullptr) {
            // The rotation can not be done
            return;
        }
    }

    m_mainRotation = (m_mainRotation + 90) % 360;

    for (std::size_t i = 0 ; i < 4 ; i++) {
        auto position = m_mainBlock[i]->getComponent("position");
        auto globalPosition = fromLocalToGlobal(positions[i]);
        position->set("x", globalPosition.x);
        position->set("y", globalPosition.y);
    }
}

std::list<unsigned int> Grid::findCompleteLines() {
    std::list<unsigned int> completeLines;

    for (int line = m_height - 1 ; line >= 0 ; line--) {
        bool completeLine = true;
        for (int column = 0 ; column < m_width ; column++) {
            if (m_grid[line][column] == nullptr) {
                completeLine = false;
            }
        }

        if (completeLine) {
            completeLines.push_back(line);
        }
    }

    return completeLines;
}

void Grid::createNewMainBlock() {
    int nextBlock = m_nextBlockGenerator(m_rng);

    std::string texture = "";
    sf::Vector2f positions[4];
    m_mainType = (BlockType)nextBlock;

    if (nextBlock == BlockType::SQUARE) {
        texture = "green.png";
        positions[0] = {m_topCenter.x, m_topCenter.y};
        positions[1] = {m_topCenter.x + 1, m_topCenter.y};
        positions[2] = {m_topCenter.x, m_topCenter.y - 1};
        positions[3] = {m_topCenter.x + 1, m_topCenter.y - 1};
    }
    else if (nextBlock == BlockType::LINE) {
        texture = "yellow.png";
        positions[0] = {m_topCenter.x, m_topCenter.y};
        positions[1] = {m_topCenter.x, m_topCenter.y - 1};
        positions[2] = {m_topCenter.x, m_topCenter.y - 2};
        positions[3] = {m_topCenter.x, m_topCenter.y - 3};
    }
    else if (nextBlock == BlockType::T) {
        texture = "grey.png";
        positions[0] = {m_topCenter.x, m_topCenter.y};
        positions[1] = {m_topCenter.x - 1, m_topCenter.y - 1};
        positions[2] = {m_topCenter.x, m_topCenter.y - 1};
        positions[3] = {m_topCenter.x + 1, m_topCenter.y - 1};
    }
    else if (nextBlock == BlockType::L) {
        texture = "red.png";
        positions[0] = {m_topCenter.x, m_topCenter.y};
        positions[1] = {m_topCenter.x, m_topCenter.y - 1};
        positions[2] = {m_topCenter.x, m_topCenter.y - 2};
        positions[3] = {m_topCenter.x + 1, m_topCenter.y - 2};
    }
    else if (nextBlock == BlockType::REVERSED_L) {
        texture = "orange.png";
        positions[0] = {m_topCenter.x, m_topCenter.y};
        positions[1] = {m_topCenter.x, m_topCenter.y - 1};
        positions[2] = {m_topCenter.x, m_topCenter.y - 2};
        positions[3] = {m_topCenter.x - 1, m_topCenter.y - 2};
    }
    else if (nextBlock == BlockType::S) {
        texture = "blue.png";
        positions[0] = {m_topCenter.x, m_topCenter.y};
        positions[1] = {m_topCenter.x - 1, m_topCenter.y};
        positions[2] = {m_topCenter.x - 1, m_topCenter.y - 1};
        positions[3] = {m_topCenter.x - 2, m_topCenter.y - 1};
    }
    else if (nextBlock == BlockType::Z) {
        texture = "lightBlue.png";
        positions[0] = {m_topCenter.x, m_topCenter.y};
        positions[1] = {m_topCenter.x + 1, m_topCenter.y};
        positions[2] = {m_topCenter.x + 1, m_topCenter.y - 1};
        positions[3] = {m_topCenter.x + 2, m_topCenter.y - 1};
    }

    // We first check if we can spawn the block in the grid, i.e., if every position is free
    // Otherwise, we send a game over event
    for (const auto &position : positions) {
        if (m_grid[position.y][position.x] != nullptr) {
            m_context.context.eventHandler->sendEvent("GameOver");
            return;
        }
    }

    for (std::size_t i = 0 ; i < 4 ; i++) {
        nlohmann::json json;
        sf::Vector2f actualPosition = fromLocalToGlobal(positions[i]);
        json["position"] = {actualPosition.x, actualPosition.y};
        json["composedsprite"]["type"] = "quads";
        json["composedsprite"]["sprite"] = texture;
        json["composedsprite"]["vertices"] = { {-1, -1}, {1, -1}, {1, 1}, {-1, 1} };
        json["composedsprite"]["scale"] = m_cellSize / 2;
        m_mainBlock[i] = m_context.entityManager->addEntity("main" + std::to_string(i), json);
    }
    m_mainRotation = 0;
}

bool Grid::canMoveDown(unsigned long x, unsigned long y) const {
    return y > 0 && m_grid[y - 1][x] == nullptr;
}

bool Grid::canMoveRight(unsigned long x, unsigned long y) const {
    return x < m_width - 1 && m_grid[y][x + 1] == nullptr;
}

bool Grid::canMoveLeft(unsigned long x, unsigned long y) const {
    return x > 0 && m_grid[y][x - 1] == nullptr;
}

sf::Vector2f Grid::fromGlobalToLocal(float x, float y) const {
    x = floor((x - m_origin.x) / m_cellSize);
    y = floor(abs(y - m_origin.y) / m_cellSize);
    return sf::Vector2f(x, y);
}

sf::Vector2f Grid::fromLocalToGlobal(sf::Vector2f localPosition) const {
    // We are always in the x-positive and y-positive part of the plan
    float x = m_origin.x + (localPosition.x * m_cellSize);
    float y = m_origin.y - (localPosition.y * m_cellSize);
    return sf::Vector2f(x, y);
}