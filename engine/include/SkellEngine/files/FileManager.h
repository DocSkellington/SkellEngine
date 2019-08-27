#pragma once

#include <filesystem>

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <map>
#include <SFML/Graphics/Font.hpp>
#include <Thor/Resources.hpp>

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
         * 
         * It loads the game description, sets the configuration of the logger and registers the Lua states and systems
         * \param context The context
         * \param baseMediapath The path to the media folder
         */
        explicit FileManager(Context &context, const std::string& baseMediapath);
        FileManager(const FileManager&) = delete;

        /**
         * \brief Returns the description defined in the media/game.json file
         * \return The Game Description
         */
        const GameDescription& getGameDescription() const;

        /**
         * \brief Gets the font "fontName"
         * 
         * The font must be in the "fonts" folder of the game description
         * \param fontName The name of the font to load
         * \return The font
         * \throws thor::ResourceLoadingException
         */
        sf::Font& loadFont(const std::string &fontName);

        /**
         * \brief Loads the sprite texture from the path
         * 
         * The texture must be in the "baseSprites" folder of the game description
         * 
         * The ID of the texture if the path
         * \param path The path to the texture to load (relative to baseSprites)
         * \param override If true, the texture is overriden by the new one
         * \return The texture
         * \throws thor::ResourceLoadingException
         */
        sf::Texture& loadSpriteTexture(const std::filesystem::path &path, bool override = false);

        /**
         * \brief Loads the texture in the given path and registers it under the textureName
         * 
         * The ID of the texture if the path
         * 
         * \param path The path to the texture (relative from the root of the media folder)
         * \param override If true, the texture is overriden by the new one
         * \return The texture
         * \throws thor::ResourceLoadingException
         */
        sf::Texture& loadTexture(const std::filesystem::path &path, bool override = false);

        /** @{ */
        /**
         * \brief Gets a texture already loaded
         * \param id The ID of the texture
         * \return The texture
         * \throws thor::ResourceAccessException
         */
        const sf::Texture& getTexture(const std::string &id) const;
        sf::Texture& getTexture(const std::string &id);
        /** @} */

        /**
         * \brief Loads a texture from an image
         * \param id The id of the texture
         * \param image The image
         * \param override If true, the texture is overriden by the new one
         * \return The texture
         */
        sf::Texture& loadTextureFromImage(const std::string &id, const sf::Image &image, bool override = false);

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

        /**
         * \brief Registers Lua functions for the file manager
         * \param lua The Lua state
         */
        void luaFunctions(sol::state &lua);

    private:
        const Context &m_context;
        GameDescription m_gameDescription;
        std::map<std::string, std::filesystem::path> m_systemsPath;
        std::map<std::string, std::filesystem::path> m_statesPath;

        /**
         * \brief The texture holder
         */
        thor::ResourceHolder<sf::Texture, std::string> m_textureHolder;
        
        /**
         * \brief The font holder
         */
        thor::ResourceHolder<sf::Font, std::string> m_fontHolder;

    private:
        const std::filesystem::path fontPath(const std::string &fontName) const;

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
    };
}