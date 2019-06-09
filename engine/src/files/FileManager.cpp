#include "SkellEngine/files/FileManager.h"

#include <fstream>
#include <cctype>
#include <algorithm>
#include <sol/sol.hpp>
#include <filesystem>

#include "SkellEngine/Context.h"
#include "SkellEngine/errors/FileNotFound.h"
#include "SkellEngine/errors/SystemNotFound.h"
#include "SkellEngine/errors/BadLevelDescription.h"
#include <SkellEngine/tmxlite/Log.hpp>

namespace engine::files {
    FileManager::FileManager(const Context &context, const std::string &baseMediapath) :
        m_context(context) {
        std::filesystem::path basePath = baseMediapath;
        std::filesystem::path gameJSONPath = basePath / "game.json";

        nlohmann::json gameJSON;

        std::ifstream file(gameJSONPath);
        if (!file.is_open()) {
            throw errors::FileNotFound("media/game.json could not be found. Please check that the base media folder path is correct and that this folder contains the file 'game.json'.");
        }
        file >> gameJSON;
        m_gameDescription = gameJSON.get<GameDescription>();

        // We concatenate the base media path and the different paths
        m_gameDescription.media.baseSprites = basePath / m_gameDescription.media.baseSprites;
        m_gameDescription.media.mapFolder = basePath / m_gameDescription.media.mapFolder;
        m_gameDescription.media.systemsFolder = basePath / m_gameDescription.media.systemsFolder;
        m_gameDescription.media.fontsFolder = basePath / m_gameDescription.media.fontsFolder;
        m_gameDescription.media.entitiesFolder = basePath / m_gameDescription.media.entitiesFolder;
        m_gameDescription.media.levelsFolder = basePath / m_gameDescription.media.levelsFolder;

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
            std::filesystem::path defaultFilePath = m_gameDescription.media.entitiesFolder;
            defaultFilePath += "data/" + entityType + ".json";
            std::ifstream defaultFile(defaultFilePath);
            std::ifstream levelGlobal;

            if (!m_levelDescription.name.empty()) {
                std::filesystem::path levelFilePath = m_gameDescription.media.levelsFolder;
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
            m_entitiesGlobal[entityType] = fusion(def, lev);
        }
        return m_entitiesGlobal[entityType];
    }

    sf::Font& FileManager::loadFont(const std::string &fontName) {
        return m_context.fontHolder->acquire(fontName, thor::Resources::fromFile<sf::Font>(fontPath(fontName)), thor::Resources::Reuse);
    }

    void FileManager::registerExternSystems() {
        if (std::filesystem::exists(m_gameDescription.media.systemsFolder)) {
            for (auto &file : std::filesystem::directory_iterator(m_gameDescription.media.systemsFolder)) {
                if (file.is_regular_file()) {
                    std::smatch match;
                    std::regex lua(".lua");

                    std::string filename = file.path().filename();

                    if (std::regex_search(filename, match, lua)) {
                        // The file ends with ".lua"
                        std::string systemName = match.prefix();

                        std::smatch isGraphical;
                        std::regex graphical("graphical", std::regex::icase);
                        if (std::regex_search(systemName, isGraphical, graphical)) {
                            // The name starts with "graphical"/i. We reject every graphical system in Lua.
                            tmx::Logger::log("Registering extern systems: a system defined in Lua can not be graphical and can not start with the word 'graphical' (case does not matter). " + filename + " is not correct. Ignoring...", tmx::Logger::Type::Warning);
                        }
                        else {
                            std::smatch matchSystemName;
                            std::regex system("[Ss]ystem$");

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
        }
        else {
            tmx::Logger::log("No external systems found", tmx::Logger::Type::Info);
        }
    }

    std::filesystem::path FileManager::getSystemPath(const std::string &systemName) {
        auto itr = m_systemsPath.find(systemName);
        if (itr != m_systemsPath.end()) {
            return m_gameDescription.media.systemsFolder / itr->second;
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
        std::ifstream file(m_gameDescription.media.statesFolder / "game.json");
        if (file.is_open()) {
            file >> game;
            m_stateDescriptions["game"] = game;
        }
        else {
            tmx::Logger::log("No game state", tmx::Logger::Type::Info);
        }
    }

    const std::filesystem::path FileManager::fontPath(const std::string &fontName) const {
        std::filesystem::path path = m_gameDescription.media.fontsFolder;
        path /= fontName + ".ttf";
        return path;
    }
}