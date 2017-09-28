#pragma once

#include <json.hpp>
#include <sol.hpp>

namespace engine {
    struct Context;

    /**
    * \brief Handles the files.
    * \todo Handle errors
    *
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
         * \brief Loads the game.json in the media folder.
         * \return The JSON table
         * \throws FileNotFound If media/game.json does not exist
         */
        nlohmann::json getGameDescription();

        /**
         * \brief Loads the default Lua script of an entity (the one in media/entities/script), if it exists.
         * \param entityType The type of the entity
         * \throws FileNotFound If the file does not exist
         */
        void loadEntityDefaultLua(const std::string &entityType);

        /**
         * \brief Loads the level-specific Lua script of an entity (the one in media/levels/levelName/entities/script), if it exists.
         * \param entityType The type of the entity
         * \return The Lua table
         * \throws FileNotFound If the file does not exist
        */
        sol::table getEntityLevelLua(const std::string &entityType);

    private:
        const Context &m_context;
        std::string m_levelName;
        nlohmann::json m_gameDescription;
        nlohmann::json m_levelDescription;
        std::map<std::string, nlohmann::json> m_entitiesGlobal;

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
    };
}