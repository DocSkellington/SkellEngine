#pragma once

#include <SFML/Graphics.hpp>

#include "SkellEngine/states/StateContext.h"
#include "SkellEngine/map/Map.h"
#include "SkellEngine/levels/LevelDescription.h"

/**
 * \brief Namespace with classes to handle a level
 */
namespace engine::levels {
    /**
     * \brief A level is defined by a map and entities
     * 
     * The map and entities to use are given in a file named "name_of_level.json" and are loaded using Level::changeLevel
     */
    class Level {
    public:
        /**
         * \brief Constructor
         * \param context The context of the state
         */
        Level(states::StateContext &context);

        /**
         * \brief Updates the level's map
         * \param deltaTime The time since the last frame
         */
        void update(sf::Int64 deltaTime);

        /**
         * \brief Draws the whole level
         * 
         * It draws the map and the entities. Actually, it calls systems::SystemManager::draw which in turn calls Level::drawLayer if a graphical system is in use
         * \param window The window to draw in
         */
        void draw(sf::RenderWindow &window);

        /**
         * \brief Draws a specific layer of the level
         * \param window The window to draw in
         * \param layer The layer to draw
         * \param view The view
         * \note This function should be called by a graphical system to draw layer by layer. The states should use Level::draw
         */
        void drawLayer(sf::RenderWindow &window, unsigned int layer, sf::View view);

        /**
         * \brief Clears the data related to the current level and loads a new level
         * \param levelName The name of the level
         */
        void changeLevel(const std::string &levelName);
        
        /**
         * \brief Registers Lua functions
         * \param lua The Lua state
         */
        void luaFunctions(sol::state &lua);
    
    private:
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
         * \brief Loads the JSON table of an entity.
         *
         * When loading an entity, it opens both the default file (if it exists) and the overriding one in the level folder (if it exists).
         * \param entityType The entity of the entity
         * \return The JSON table
         */
        nlohmann::json getEntityJSON(const std::string &entityType);

    private:
        states::StateContext &m_context;
        map::Map m_map;
        LevelDescription m_levelDescription;
        std::map<std::string, nlohmann::json> m_entitiesGlobal;
    };
}