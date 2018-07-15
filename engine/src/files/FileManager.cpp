#include "files/FileManager.h"

#include <fstream>
#include <cctype>
#include <algorithm>
#include <sol.hpp>
#include <filesystem>

#include "Context.h"
#include "errors/FileNotFound.h"
#include "errors/SystemNotFound.h"
#include "errors/BadLevelDescription.h"
#include <tmxlite/Log.hpp>

namespace engine::files {
    FileManager::FileManager(const Context &context) :
        m_context(context) {
        nlohmann::json gameJSON;
        std::ifstream file("media/game.json");
        if (!file.is_open()) {
            throw errors::FileNotFound("media/game.json could not be found. Please check that the media folder is complete and correct.");
        }
        file >> gameJSON;
        m_gameDescription = gameJSON.get<GameDescription>();
        loadStateDescriptions();

        registerExternSystems();
    }

    void FileManager::changeLevel(const std::string& levelName) {
        tmx::Logger::log("Changing level to: " + levelName, tmx::Logger::Type::Info);
        // Clearing 
        m_levelDescription.clear();
        m_entitiesGlobal.clear();

        m_levelDescription.name = levelName;

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

            if (!m_levelDescription.name.empty()) {
                levelGlobal.open("media/levels/" + m_levelDescription.name + "/entities/data/" + entityType + ".json");
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

    sf::Font& FileManager::loadFont(const std::string &fontName) {
        return m_context.fontHolder->acquire("fonts/" + fontName, thor::Resources::fromFile<sf::Font>(fontPath(fontName)), thor::Resources::Reuse);
    }

    void FileManager::registerExternSystems() {
        for (auto &file : std::filesystem::directory_iterator(m_gameDescription.media.systemsFolder)) {
            if (file.is_regular_file()) {
                std::smatch match;
                std::regex lua(".lua");

                std::string filename = file.path().filename();

                if (std::regex_search(filename, match, lua)) {
                    // The file ends with ".lua"
                    std::smatch matchSystemName;
                    std::regex system("[Ss]ystem$");

                    std::string systemName = match.prefix();

                    if (std::regex_search(systemName, matchSystemName, system)) {
                        // The name of the file is "BlablaSystem.lua"; we want to keep "Blabla"
                        systemName = matchSystemName.prefix();
                    }

                    std::transform(systemName.begin(), systemName.end(), systemName.begin(), ::tolower);

                    m_systemsPath[systemName] = filename;

                    tmx::Logger::log("Registering " + systemName);
                }
            }
        }
    }

    std::string FileManager::getSystemPath(const std::string &systemName) {
        auto itr = m_systemsPath.find(systemName);
        if (itr != m_systemsPath.end()) {
            return m_gameDescription.media.systemsFolder + itr->second;
        }
        else {
            throw errors::SystemNotFound("The system " + systemName + " is unknown. Please check that the Lua script is in the correct directory and that the filename is correct.");
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
        if (!m_levelDescription.name.empty()) {
            std::ifstream file;
            file.open("media/levels/" + m_levelDescription.name + "/" + m_levelDescription.name + ".json");

            if (file.is_open()) {
                nlohmann::json levelJSON;
                file >> levelJSON;
                m_levelDescription = levelJSON.get<LevelDescription>();
            }
        }
    }

    void FileManager::applyLevelDescription() {
        m_context.map->load(m_levelDescription.map);

        auto &entities = m_levelDescription.entities;

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

    const std::string FileManager::fontPath(const std::string &fontName) const {
        std::string path = "media/fonts/" + fontName + ".ttf";
        return path;
    }
}