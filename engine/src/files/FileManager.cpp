#include "SkellEngine/files/FileManager.h"

#include <fstream>
#include <cctype>
#include <algorithm>
#include <sol/sol.hpp>
#include <filesystem>

#include "SkellEngine/errors/FileNotFound.h"
#include "SkellEngine/errors/SystemNotFound.h"
#include "SkellEngine/errors/StateNotFound.h"
#include "SkellEngine/errors/BadLevelDescription.h"
#include "SkellEngine/tmxlite/Log.hpp"

namespace engine::files {
    FileManager::FileManager(const Context &context, const std::string &baseMediapath) :
        m_context(context) {
        std::filesystem::path basePath = baseMediapath;
        std::filesystem::path gameJSONPath = basePath / "game.json";

        nlohmann::json gameJSON;

        std::ifstream file(gameJSONPath);
        if (!file.is_open()) {
            throw errors::FileNotFound(gameJSONPath.string() + " could not be found. Please check that the base media folder path is correct and that this folder contains the file 'game.json'.");
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
        m_gameDescription.media.statesFolder = basePath / m_gameDescription.media.statesFolder;
        m_gameDescription.media.inputDescription = basePath / m_gameDescription.media.inputDescription;

        loadStateDescriptions();

        registerExternSystems();
        registerExternStates();
    }

    const GameDescription& FileManager::getGameDescription() const {
        return m_gameDescription;
    }

    const StateDescription& FileManager::getStateDescription(const std::string &state) {
        return m_stateDescriptions[state];
    }

    sf::Font& FileManager::loadFont(const std::string &fontName) {
        return m_context.fontHolder->acquire(fontName, thor::Resources::fromFile<sf::Font>(fontPath(fontName)), thor::Resources::Reuse);
    }

    void FileManager::registerExternSystems() {
        if (std::filesystem::exists(m_gameDescription.media.systemsFolder) && std::filesystem::is_directory(m_gameDescription.media.systemsFolder)) {
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

                            m_systemsPath[systemName] = file.path();

                            tmx::Logger::log("Registering system " + systemName);
                        }
                    }
                }
            }
        }
        else {
            tmx::Logger::log("No external systems found", tmx::Logger::Type::Info);
        }
    }

    void FileManager::registerExternStates() {
        tmx::Logger::log(m_gameDescription.media.statesFolder);
        if (std::filesystem::exists(m_gameDescription.media.statesFolder) && std::filesystem::is_directory(m_gameDescription.media.statesFolder)) {
            for (auto &file : std::filesystem::directory_iterator(m_gameDescription.media.statesFolder)) {
                if (file.is_regular_file()) {
                    std::string filename = file.path().filename();

                    std::smatch matchLua;
                    std::regex lua(".lua");
                    if (std::regex_search(filename, matchLua, lua)) {
                        // The file ends with ".lua"
                        std::string stateName = matchLua.prefix();

                        std::smatch matchStateName;
                        std::regex state("[Ss]tate$");

                        if (std::regex_search(stateName, matchStateName, state)) {
                            // The name of the file is "Blablastate.lua"; we keep only "Blabla"
                            stateName = matchStateName.prefix();
                        }

                        std::transform(stateName.begin(), stateName.end(), stateName.begin(), ::tolower);
                        tmx::Logger::log(stateName);
                        m_statesPath[stateName] = file.path();

                        tmx::Logger::log("Registering state " + stateName);
                    }
                }
            }
        }
        else {
            tmx::Logger::log("No external states found", tmx::Logger::Type::Info);
        }
    }

    std::filesystem::path FileManager::getSystemPath(const std::string &systemName) {
        auto itr = m_systemsPath.find(systemName);
        if (itr != m_systemsPath.end()) {
            return itr->second;
        }
        else {
            throw errors::SystemNotFound("The system " + systemName + " is unknown. Please check that the Lua script is in the correct directory and that the filename is correct.");
        }
    }

    std::filesystem::path FileManager::getStatePath(const std::string &stateName) {
        auto itr = m_statesPath.find(stateName);
        if (itr != m_statesPath.end()) {
            return itr->second;
        }
        else {
            throw errors::StateNotFound("The state " + stateName + " is unknown. Please check that the Lua script is in the correct directory and that the filename is correct.");
        }
    }

    void FileManager::loadStateDescriptions() {
        /**\todo The other states */
        nlohmann::json mainmenu, game, pause;
        std::filesystem::path gamePath = m_gameDescription.media.statesFolder / "game.json";
        std::ifstream file(gamePath);
        if (file.is_open()) {
            file >> game;
            m_stateDescriptions["game"] = game;
        }
        else {
            tmx::Logger::log("No game state", tmx::Logger::Type::Warning);
        }
    }

    const std::filesystem::path FileManager::fontPath(const std::string &fontName) const {
        std::filesystem::path path = m_gameDescription.media.fontsFolder;
        path /= fontName + ".ttf";
        return path;
    }
}