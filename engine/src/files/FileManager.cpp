#include "files/FileManager.h"

#include <fstream>
#include <cctype>
#include <algorithm>

#include "Context.h"
#include "errors/FileNotFound.h"

namespace engine::files {
    FileManager::FileManager(const Context &context) :
        m_context(context) {
        nlohmann::json gameJSON;
        std::ifstream file("media/game.json");
        file >> gameJSON;
        m_gameDescription = gameJSON.get<GameDescription>();
        loadStateDescriptions();
    }

    void FileManager::changeLevel(const std::string& levelName) {
        // Clearing 
        m_levelDescription.clear();
        m_entitiesGlobal.clear();

        m_levelName = levelName;

        // Loading
        loadLevelDescription();
        applyLevelDescription();
    }

    const GameDescription& FileManager::getGameDescription() const {
        return m_gameDescription;
    }

    const StateDescription& FileManager::getStateDescription(const std::string &state) {
        return m_stateDescriptions[state];
    }

    nlohmann::json FileManager::getEntityJSON(const std::string &entityType) {
        if (m_entitiesGlobal.find(entityType) == m_entitiesGlobal.end()) {
            // If the entity is not yet known, we try to load the global settings (the global for the game and for the level), if they exist
            std::ifstream defaultFile("media/entities/data/" + entityType + ".json");
            std::ifstream levelGlobal;

            if (!m_levelName.empty()) {
                levelGlobal.open("media/levels/" + m_levelName + "/entities/data/" + entityType + ".json");
            }

            nlohmann::json def, lev;
            if (defaultFile.is_open()) {
                defaultFile >> def;
            }
            if (levelGlobal.is_open()) {
                levelGlobal >> lev;
            }
            m_entitiesGlobal[entityType] = fusion(def, lev);
        }
        return m_entitiesGlobal[entityType];
    }

    void FileManager::loadEntityDefaultLua(const std::string &entityType) {
        if (!((*m_context.lua)[entityType].valid()))
            m_context.lua->safe_script_file("media/entities/scripts/" + entityType + ".lua");
    }

    sol::table FileManager::getEntityLevelLua(const std::string &entityType) {
        if (!m_levelName.empty()) {
            if (!((*m_context.lua)[entityType].valid())) {
                // If there isn't any script linked to that entity, we load the level one
                m_context.lua->safe_script_file("media/levels/" + m_levelName + "/entities/scripts/" + entityType + ".lua");
            }
            else {
                // If there is already a script, we merge them
                sol::load_result script = m_context.lua->load_file("media/levels/" + m_levelName + "/entities/scripts" + entityType + ".lua");

                // TODO: read every value. If table, go in it. Copy/overwrite data in m_context.lua
            }
        }
    }

    nlohmann::json FileManager::fusion(const nlohmann::json &a, const nlohmann::json &b) const {
        nlohmann::json result(a);
        for (auto itr = b.begin() ; itr != b.end() ; ++itr) {
            result[itr.key()] = itr.value();
        }
        return result;
    }

    void FileManager::loadLevelDescription() {
        if (!m_levelName.empty()) {
            std::ifstream file;
            file.open("media/levels/" + m_levelName + "/" + m_levelName + ".json");

            if (file.is_open()) {
                file >> m_levelDescription;
            }
        }
    }

    void FileManager::applyLevelDescription() {
        auto &entities = m_levelDescription["entities"];

        for (auto itr = entities.begin() ; itr != entities.end() ; ++itr) {
            // Getting the type of the entity
            std::string type = (*itr)["type"].get<std::string>();
            // Getting its components' data
            auto data = (*itr)["data"];
            // We add these data into the global definition known
            auto entityGlobal = getEntityJSON(type);
            auto entity = fusion(entityGlobal, data);
            // Finally, we create the new entity
            m_context.entityManager->addEntity(type, entity);
        }
    }

    void FileManager::loadStateDescriptions() {
        /**\todo The other states */
        nlohmann::json mainmenu, game, pause;
        std::ifstream file("media/states/game.json");
        file >> game;
        m_stateDescriptions["game"] = game;
    }
}