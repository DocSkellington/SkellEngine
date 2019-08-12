#include "SkellEngine/levels/Level.h"

#include "SkellEngine/files/FileManager.h"
#include "SkellEngine/utilities/json_fusion.h"
#include "SkellEngine/errors/FileNotFound.h"
#include "SkellEngine/entities/EntityManager.h"
#include "SkellEngine/systems/SystemManager.h"

namespace engine::levels {
    Level::Level(states::StateContext &context) :
        m_context(context),
        m_map(context) {

    }

    void Level::update(sf::Int64 deltaTime) {
        m_map.updateLayers(deltaTime);
    }

    void Level::draw(sf::RenderWindow &window) {
        for (std::size_t i = 0 ; i < m_map.getLayerCount() ; i++) {
            m_context.systemManager->draw(window, i);
        }
    }

    void Level::drawLayer(sf::RenderWindow &window, unsigned int layer, sf::View view) {
        m_map.drawLayer(window, layer, view);
    }

    void Level::changeLevel(const std::string& levelName) {
        tmx::Logger::log("Changing level to: " + levelName, tmx::Logger::Type::Info);
        // Clearing 
        m_levelDescription.clear();
        m_entitiesGlobal.clear();

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

        auto &entities = m_levelDescription.entities;

        for (auto itr = entities.begin() ; itr != entities.end() ; ++itr) {
            // Getting the type of the entity
            std::string type = (*itr)["type"].get<std::string>();
            // Getting its components' data
            auto data = (*itr)["data"];
            // We add these data into the global definition known
            auto entityGlobal = getEntityJSON(type);
            auto entity = utilities::json_fusion(entityGlobal, data);
            // Finally, we create the new entity
            m_context.entityManager->addEntity(type, entity);
        }
    }

    nlohmann::json Level::getEntityJSON(const std::string &entityType) {
        auto gameDescription = m_context.context.fileManager->getGameDescription();

        if (m_entitiesGlobal.find(entityType) == m_entitiesGlobal.end()) {
            // If the entity is not yet known, we try to load the global settings (the global for the game and for the level), if they exist
            std::filesystem::path defaultFilePath = gameDescription.media.entitiesFolder;
            defaultFilePath += "data/" + entityType + ".json";
            std::ifstream defaultFile(defaultFilePath);
            std::ifstream levelGlobal;

            if (!m_levelDescription.name.empty()) {
                std::filesystem::path levelFilePath = gameDescription.media.levelsFolder;
                levelFilePath /= m_levelDescription.name + "/entities/data" + entityType + ".json";
                levelGlobal.open(levelFilePath);
            }

            nlohmann::json def, lev;
            if (defaultFile.is_open()) {
                defaultFile >> def;
            }
            if (levelGlobal.is_open()) {
                levelGlobal >> lev;
            }
            m_entitiesGlobal[entityType] = utilities::json_fusion(def, lev);
        }
        return m_entitiesGlobal[entityType];
    }
}