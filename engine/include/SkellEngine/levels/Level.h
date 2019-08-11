#pragma once

#include <SFML/Graphics.hpp>

#include "SkellEngine/states/StateContext.h"
#include "SkellEngine/map/Map.h"
#include "SkellEngine/levels/LevelDescription.h"

/**
 * \brief Namespace with classes to handle a level
 */
namespace engine::levels {
    class Level {
    public:
        Level(states::StateContext &context);

        void update(sf::Int64 deltaTime);

        /**
         * \brief Draws the whole level
         */
        void draw(sf::RenderWindow &window);

        /**
         * \brief Draws a specific layer of the level
         */
        void drawLayer(sf::RenderWindow &window, unsigned int layer, sf::View view);

        /**
         * \brief Clears the data related to the current level and loads a new level
         * \param levelName The name of the level
         */
        void changeLevel(const std::string &levelName);
    
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