#pragma once

#include <filesystem>

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <map>
#include <SFML/Graphics/Font.hpp>

#include "SkellEngine/files/GameDescription.h"
#include "SkellEngine/files/StateDescription.h"
#include "SkellEngine/files/LevelDescription.h"

namespace engine {
    struct Context;
}

/**
 * \brief Contains the ways to use the external files
 */
namespace engine::files {
    /**
    * \brief Handles the files.
    * \ingroup Engine
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
         * \brief Clears the data related to the current level and loads a new level
         * \param levelName The name of the level
         */
        void changeLevel(const std::string &levelName);

        /**
         * \brief Returns the description defined in the media/game.json file
         * \return The Game Description
         */
        const GameDescription& getGameDescription() const;

        /**
         * \brief Returns the description of the state found in the media/states folder
         * \return The StateDescription of the given state
         */
        const StateDescription& getStateDescription(const std::string &state);

        /**
         * \brief Loads a font and stores it in the appropriate ressource holder.
         * \param fontName The name of the font to load
         * \return The loaded font
         */
        sf::Font& loadFont(const std::string &fontName);

        /**
         * \brief Register the systems written in Lua.
         * 
         * Every external system must be in the folder defined in game.json (media.systems field)
         * \see @ref json_game_description
         */
        void registerExternSystems();

        /**
         * \brief Gets the path to the given system
         * 
         * The system must be an external system
         * \param systemName The name of the system
         * \return The path
         */
        std::filesystem::path getSystemPath(const std::string &systemName);

    private:
        const Context &m_context;
        LevelDescription m_levelDescription;
        std::map<std::string, nlohmann::json> m_entitiesGlobal;
        GameDescription m_gameDescription;
        std::map<std::string, StateDescription> m_stateDescriptions;
        std::map<std::string, std::string> m_systemsPath;

    private:
        /**
         * \brief Loads the JSON table of an entity.
         *
         * When loading an entity, it opens both the default file (if it exists) and the overriding one in the level folder (if it exists).
         * \param entityType The entity of the entity
         * \return The JSON table
         */
        nlohmann::json getEntityJSON(const std::string &entityType);

        /**
         * \brief Opens the media/levels/levelName/levelName.json file and copies its content into the level description
         */
        void loadLevelDescription();
        /**
         * \brief Reads the level description and applies the content to the engine, if needed.
         * 
         * Creates entities defined in the JSON table
         */
        void applyLevelDescription();

        /**
         * \brief Loads the state descriptions in media/states
         */
        void loadStateDescriptions();

        const std::filesystem::path fontPath(const std::string &fontName) const;
    };
}