#pragma once

#include <filesystem>

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <map>
#include <SFML/Graphics/Font.hpp>

#include "SkellEngine/files/GameDescription.h"
#include "SkellEngine/Context.h"

/**
 * \brief Contains the ways to use the external files
 */
namespace engine::files {
    /**
    * \brief Handles the files.
    */
    class FileManager {
    public:
        /**
         * The constructor
         * \param context The context
         * \param baseMediapath The path to the folder containing the "media" folder
         */
        explicit FileManager(const Context &context, const std::string& baseMediapath);
        FileManager(const FileManager&) = delete;

        /**
         * \brief Returns the description defined in the media/game.json file
         * \return The Game Description
         */
        const GameDescription& getGameDescription() const;

        /**
         * \brief Loads a font and stores it in the appropriate ressource holder.
         * \param fontName The name of the font to load
         * \return The loaded font
         */
        sf::Font& loadFont(const std::string &fontName);

        /**
         * \brief Register the systems written in Lua.
         * 
         * Every external system must be in the folder defined in game.json (media.systemsFolder field)
         * \see @ref json_game_description
         */
        void registerExternSystems();

        /**
         * \brief Register the states written in Lua
         * 
         * Every external state must be in the folder defined in game.json (media.statesFolder field)
         */
        void registerExternStates();

        /**
         * \brief Gets the path to the given system
         * 
         * The system must be an external system
         * \param systemName The name of the system
         * \return The path
         */
        std::filesystem::path getSystemPath(const std::string &systemName);

        /**
         * \brief Gets the path to the given state
         * 
         * The state must be an external state
         * \param stateName The name of the state
         * \return The path
         */
        std::filesystem::path getStatePath(const std::string &stateName);

    private:
        const Context &m_context;
        GameDescription m_gameDescription;
        std::map<std::string, std::filesystem::path> m_systemsPath;
        std::map<std::string, std::filesystem::path> m_statesPath;

    private:
        const std::filesystem::path fontPath(const std::string &fontName) const;
    };
}