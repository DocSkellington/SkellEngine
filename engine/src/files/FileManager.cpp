#include "SkellEngine/files/FileManager.hpp"

#include <fstream>
#include <cctype>
#include <algorithm>
#include <sol/sol.hpp>
#include <filesystem>

#include "SkellEngine/errors/FileNotFound.hpp"
#include "SkellEngine/errors/SystemNotFound.hpp"
#include "SkellEngine/errors/StateNotFound.hpp"
#include "SkellEngine/errors/BadLevelDescription.hpp"

namespace engine::files {
    FileManager::FileManager(Context &context, const std::string &baseMediapath) :
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
        m_gameDescription.media.baseMediaPath = basePath;
        m_gameDescription.media.baseSprites = basePath / m_gameDescription.media.baseSprites;
        m_gameDescription.media.mapFolder = basePath / m_gameDescription.media.mapFolder;
        m_gameDescription.media.systemsFolder = basePath / m_gameDescription.media.systemsFolder;
        m_gameDescription.media.fontsFolder = basePath / m_gameDescription.media.fontsFolder;
        m_gameDescription.media.entitiesFolder = basePath / m_gameDescription.media.entitiesFolder;
        m_gameDescription.media.levelsFolder = basePath / m_gameDescription.media.levelsFolder;
        m_gameDescription.media.statesFolder = basePath / m_gameDescription.media.statesFolder;
        m_gameDescription.media.inputDescription = basePath / m_gameDescription.media.inputDescription;

        m_gameDescription.log.logPath = basePath / m_gameDescription.log.logPath;

        context.logger.setLogLevel(m_gameDescription.log.level);
        context.logger.setLogOutput(m_gameDescription.log.output);
        context.logger.setOutputFile(m_gameDescription.log.logPath);

        registerExternSystems();
        registerExternStates();
    }

    const GameDescription& FileManager::getGameDescription() const {
        return m_gameDescription;
    }

    sf::Font& FileManager::loadFont(const std::string &fontName) {
        return m_fontHolder.acquire(fontName, thor::Resources::fromFile<sf::Font>(fontPath(fontName)), thor::Resources::Reuse);
    }

    sf::Texture& FileManager::loadSpriteTexture(const std::filesystem::path &path, bool override) {
        return m_textureHolder.acquire(
            path.string(),
            thor::Resources::fromFile<sf::Texture>(m_gameDescription.media.baseSprites / path.string()),
            override ? thor::Resources::KnownIdStrategy::Reload : thor::Resources::KnownIdStrategy::Reuse
        );
    }

    sf::Texture& FileManager::loadTexture(const std::filesystem::path &path, bool override) {
        return m_textureHolder.acquire(
            path.string(),
            thor::Resources::fromFile<sf::Texture>(m_gameDescription.media.baseMediaPath / path.string()),
            override ? thor::Resources::KnownIdStrategy::Reload : thor::Resources::KnownIdStrategy::Reuse
        );
    }

    sf::Texture& FileManager::getTexture(const std::string &id) {
        return m_textureHolder[id];
    }

    const sf::Texture& FileManager::getTexture(const std::string &id) const {
        return m_textureHolder[id];
    }

    sf::Texture& FileManager::loadTextureFromImage(const std::string &id, const sf::Image &image, bool override) {
        return m_textureHolder.acquire(
            id, 
            thor::Resources::fromImage<sf::Texture>(image),
            override ? thor::Resources::KnownIdStrategy::Reload : thor::Resources::KnownIdStrategy::Reuse
        );
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
                            // The name starts with "graphical". We reject every graphical system in Lua.
                            m_context.logger.log("Registering extern systems: a system defined in Lua can not be graphical and can not start with the word 'graphical' (case does not matter). " + filename + " is not correct.", LogType::Warning);
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

                            m_context.logger.log("Registering system " + systemName);
                        }
                    }
                }
            }
        }
        else {
            m_context.logger.log("No external systems found", LogType::Info);
        }
    }

    void FileManager::registerExternStates() {
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
                        m_context.logger.log(stateName);
                        m_statesPath[stateName] = file.path();

                        m_context.logger.log("Registering state " + stateName);
                    }
                }
            }
        }
        else {
            m_context.logger.log("No external states found", LogType::Info);
        }
    }

    std::filesystem::path FileManager::getSystemPath(const std::string &systemName) {
        auto itr = m_systemsPath.find(systemName);
        if (itr != m_systemsPath.end()) {
            return itr->second;
        }
        else {
            throw errors::SystemNotFound("The system " + systemName + " is unknown. Please check that the Lua script is in the correct directory and that the system name is correct. If you want to use a C++ system, make sure you registered the system under the correct name.");
        }
    }

    std::filesystem::path FileManager::getStatePath(const std::string &stateName) {
        auto itr = m_statesPath.find(stateName);
        if (itr != m_statesPath.end()) {
            return itr->second;
        }
        else {
            throw errors::StateNotFound("The state " + stateName + " is unknown. Please check that the Lua script is in the correct directory and that the state name is correct. If you want to use a C++ state, make sure you registered the state under the correct name.");
        }
    }

    void FileManager::luaFunctions(sol::state &lua) {
        lua.new_usertype<FileManager>("FileManager",
            sol::no_constructor,
            "getGameDescription", &FileManager::getGameDescription
        );

        m_gameDescription.luaFunctions(lua);

        lua["game"]["fileManager"] = this;
        lua["game"]["gameDescription"] = m_gameDescription;
    }

    const std::filesystem::path FileManager::fontPath(const std::string &fontName) const {
        std::filesystem::path path = m_gameDescription.media.fontsFolder;
        path /= fontName + ".ttf";
        return path;
    }
}