#include "SkellEngine/Context.h"

#include <sol/sol.hpp>
#include <nlohmann/json.hpp>

#include "SkellEngine/errors/FileNotFound.h"
#include "SkellEngine/config.h"
#include "SkellEngine/events/EventHandler.h"
#include "SkellEngine/input/InputHandler.h"
#include "SkellEngine/files/FileManager.h"
#include "SkellEngine/states/StateManager.h"

namespace engine {
    Context::Context(const std::filesystem::path &baseMediaPath, bool graphical) {
        // Erasing the old log
        std::filesystem::path logPath = baseMediaPath / "log.txt";
        std::filesystem::remove(logPath);

        lua = std::make_shared<sol::state>();
        lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);

        fileManager = std::make_shared<files::FileManager>(*this, baseMediaPath);
        
        logger.log("File manager ready");

        textureHolder = std::make_shared<thor::ResourceHolder<sf::Texture, std::string>>();
        logger.log("Texture holder ready");

        fontHolder = std::make_shared<thor::ResourceHolder<sf::Font, std::string>>();
        logger.log("Font holder ready");

        stateManager = std::make_shared<states::StateManager>(*this);
        logger.log("State manager ready");

        eventHandler = std::make_shared<events::EventHandler>(*this);
        logger.log("Event handler ready");

        inputHandler = std::make_shared<input::InputHandler>(*this);
        logger.log("Input handler ready");

        std::ifstream inputFile(fileManager->getGameDescription().media.inputDescription);
        if (!inputFile.is_open()) {
            throw errors::FileNotFound(fileManager->getGameDescription().media.inputDescription.string() + " could not be found.");
        }
        nlohmann::json inputDescription;
        inputFile >> inputDescription;
        inputHandler->loadConfiguration(inputDescription);

        if (graphical) {
            // Creating the window
            createWindow(fileManager->getGameDescription().window, fileManager->getGameDescription().version);
            logger.log("Window created");
        }
    }

    void Context::closeEngine() {
        logger.log("Closing the window");
        window->close();
    }

    void Context::luaFunctions(sol::state &lua) {
        lua["game"]["closeEngine"] = [this]() { this->closeEngine(); };
    }

    void Context::createWindow(const files::GameDescription::WindowDescription &windowDescription, const std::string &version) {
        std::string title = windowDescription.title;
        if (windowDescription.version)
            title += " - v." + version;
        if (windowDescription.engine)
            title += " - ENGINE v." + std::to_string(ENGINE_VERSION_MAJOR) + "." + std::to_string(ENGINE_VERSION_MINOR) + "." + std::to_string(ENGINE_VERSION_REVISION);

        int style = sf::Style::Titlebar;
        if (windowDescription.resize)
            style |= sf::Style::Resize;
        if (windowDescription.close)
            style |= sf::Style::Close;
        if (!windowDescription.titlebar)
            style = sf::Style::None;

        sf::ContextSettings settings;
        settings.antialiasingLevel = windowDescription.antialiasingLevel;

        if (windowDescription.fullscreen)
            window = std::make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen, settings);
        else
            window = std::make_shared<sf::RenderWindow>(sf::VideoMode(windowDescription.width, windowDescription.height, 32), title, style, settings);

        window->setKeyRepeatEnabled(false);

        if (windowDescription.verticalSynchronisation) {
            window->setVerticalSyncEnabled(true);
        }
        else if (windowDescription.FPS > 0) {
            window->setFramerateLimit(windowDescription.FPS);
        }
    }
}