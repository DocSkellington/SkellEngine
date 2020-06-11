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

        for (const auto &entity : entities) {
            nlohmann::json description = entity.description;
            // We get the type of the entity to create
            std::string type;
            if (auto t = description.find("type") ; t != description.end()) {
                type = t->get<std::string>();
                description.erase(t);
            }
            // We merge the specific data for this instance and the generic data for the type
            auto entityGlobal = getEntityJSON(type);
            auto entityDescription = utilities::json_fusion(entityGlobal, description);
            // Finally, we create the new entity
            m_context.entityManager->addEntity(entity.name, entityDescription);
        }
    }

    nlohmann::json Level::getEntityJSON(const std::string &entityType) {
        auto gameDescription = m_context.context.fileManager->getGameDescription();

        if (m_entitiesGlobal.find(entityType) == m_entitiesGlobal.end()) {
            // If the entity is not yet known, we try to load the global settings (the global for the game and for the level), if they exist
            std::filesystem::path defaultFilePath = gameDescription.media.entitiesFolder;
            defaultFilePath /= entityType + ".json";
            std::ifstream defaultFile(defaultFilePath);
            std::ifstream levelGlobal;

            if (!m_levelDescription.name.empty()) {
                std::filesystem::path levelFilePath = gameDescription.media.levelsFolder;
                levelFilePath /= m_levelDescription.name + "/entities/" + entityType + ".json";
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