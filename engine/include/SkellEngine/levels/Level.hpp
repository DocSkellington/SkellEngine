#pragma once

#include <SFML/Graphics.hpp>

#include "SkellEngine/states/StateContext.hpp"
#include "SkellEngine/map/Map.hpp"
#include "SkellEngine/levels/LevelDescription.hpp"

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
         * \param target The target to draw in
         */
        void draw(sf::RenderTarget &target);

        /**
         * \brief Draws a specific layer of the level
         * \param target The target to draw in
         * \param layer The layer to draw
         * \param view The view
         * \note This function should be called by a graphical system to draw layer by layer. The states should use Level::draw
         */
        void drawLayer(sf::RenderTarget &target, unsigned int layer, sf::View view);

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

        /** @{ */
        /**
         * \brief Gets the map
         * \return The map
         */
        map::Map &getMap();
        const map::Map &getMap() const;
        /** @} */

        /**
         * \brief Converts a level position (i.e., a position relative to the tiles in the map) to an absolute position.
         * 
         * The absolute position is computed by multiplying the level position by the size of a tile.
         * \param x The x position
         * \param y The y position
         * \return The absolute position
         */
        sf::Vector2f levelPositionToAbsolutePosition(double x, double y) const;

        /**
         * \brief Converts an absolute position to a level position (i.e., a position relative to the tiles in the map).
         * 
         * The relative position is computed by dividing the absolute position by the size of a tile.
         * \param x The x position
         * \param y The y position
         * \return The level position
         */
        sf::Vector2f absolutePositionToLevelPosition(double x, double y) const;
    
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

    private:
        states::StateContext &m_context;
        map::Map m_map;
        LevelDescription m_levelDescription;
        std::map<std::string, nlohmann::json> m_entitiesDefinitions;
    };
}