#pragma once

#include <json.hpp>
#include <sol.hpp>
#include <map>
#include <SFML/Graphics/Font.hpp>

#include "files/GameDescription.h"
#include "files/StateDescription.h"
#include "files/LevelDescription.h"

namespace engine {
    struct Context;
}

/**
 * \brief Contains the ways to use the external files
 */
namespace engine::files {
    /**
    * \brief Handles the files.
    */
    class FileManager {
    public:
        explicit FileManager(const Context &context);
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

        void registerExternSystems();

        std::string getSystemPath(const std::string &systemName);

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
         * \brief Fuses a and b. Be warn that b overrides data in a!
         * \param a The base json
         * \param b The overwriting json
         * \return The fusion of a and b (where common keys have b values)
        */
        nlohmann::json fusion(const nlohmann::json &a, const nlohmann::json &b) const;

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

        const std::string fontPath(const std::string &fontName) const;
    };
}