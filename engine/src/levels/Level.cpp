#include "SkellEngine/levels/Level.hpp"

#include "SkellEngine/files/FileManager.hpp"
#include "SkellEngine/utilities/json_fusion.hpp"
#include "SkellEngine/errors/FileNotFound.hpp"
#include "SkellEngine/entities/EntityManager.hpp"
#include "SkellEngine/systems/SystemManager.hpp"

namespace engine::levels {
    Level::Level(states::StateContext &context) :
        m_context(context),
        m_map(context) {

    }

    void Level::update(sf::Int64 deltaTime) {
        m_map.updateLayers(deltaTime);
    }

    void Level::draw(sf::RenderTarget &window) {
        for (std::size_t i = 0 ; i < m_map.getLayerCount() ; i++) {
            m_context.systemManager->draw(window, i);
        }
    }

    void Level::drawLayer(sf::RenderTarget &target, unsigned int layer, sf::View view) {
        m_map.drawLayer(target, layer, view);
    }

    void Level::changeLevel(const std::string& levelName) {
        m_context.context.logger.log("Changing level to: " + levelName, LogType::Info);
        // Clearing 
        m_levelDescription.clear();
        m_entitiesDefinitions.clear();

        m_levelDescription.name = levelName;

        // Loading
        loadLevelDescription();
        applyLevelDescription();
    }

    void Level::luaFunctions(sol::state &lua) {
        lua.new_usertype<Level>("Level",
            "update", &Level::update,
            "draw", &Level::draw,
            "changeLevel", &Level::changeLevel
        );

        lua["game"]["level"] = this;
    }

    map::Map &Level::getMap() {
        return m_map;
    }

    const map::Map &Level::getMap() const {
        return m_map;
    }

    sf::Vector2f Level::levelPositionToAbsolutePosition(double x, double y) const {
        auto tileSize = m_map.getTileSize();
        return sf::Vector2f(x * tileSize.x, y * tileSize.y);
    }

    sf::Vector2f Level::absolutePositionToLevelPosition(double x, double y) const {
        auto tileSize = m_map.getTileSize();
        return sf::Vector2f(x / tileSize.x, y / tileSize.y);
    }

    void Level::loadLevelDescription() {
        if (!m_levelDescription.name.empty()) {
            std::ifstream file;
            std::filesystem::path levelPath = m_context.context.fileManager->getGameDescription().media.levelsFolder / m_levelDescription.name / m_levelDescription.name;
            levelPath += ".json";
            file.open(levelPath);

            if (file.is_open()) {
                nlohmann::json levelJSON;
                file >> levelJSON;
                m_levelDescription = levelJSON.get<LevelDescription>();
            }
            else {
                throw errors::FileNotFound("Level" + m_levelDescription.name + " not found");
            }
        }
    }

    void Level::applyLevelDescription() {
        m_map.load(m_levelDescription.map);
        auto gameDescription = m_context.context.fileManager->getGameDescription();

        auto &entities = m_levelDescription.entities;

        for (const auto &entity : entities) {
            nlohmann::json description = entity.description;
            // We get the type of the entity to create
            std::string type;
            if (auto t = description.find("type") ; t != description.end()) {
                type = t->get<std::string>();
                description.erase(t);
            }

            // We load the descriptions specific for this level
            nlohmann::json levelDescription;
            if (m_entitiesDefinitions.find(type) == m_entitiesDefinitions.end()) {
                // The entity is not yet known. Thus, we load it from a file, if it exists
                if (std::ifstream levelFile ; !m_levelDescription.name.empty()) {
                    std::filesystem::path levelFilePath = gameDescription.media.levelsFolder;
                    levelFilePath /= m_levelDescription.name + "/entities/" + type + ".json";
                    levelFile.open(levelFilePath);

                    levelFile >> levelDescription;
                    m_entitiesDefinitions[type] = levelDescription;
                }
            }
            else {
                levelDescription = m_entitiesDefinitions[type];
            }
            // Finally, we can add the entity
            m_context.entityManager->addEntity(entity.name, type, levelDescription);
        }
    }
}