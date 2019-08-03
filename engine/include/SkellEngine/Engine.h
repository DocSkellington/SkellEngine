#pragma once

#include "SkellEngine/Context.h"
#include "SkellEngine/files/GameDescription.h"

/**
* \brief This numespace contains everything the engine needs
*/
namespace engine {
    /**
    * \brief The class that effectively starts the engine
    */
    class Engine final {
    public:
        /**
         * The constructor
         * \param baseMediapath The path to the folder containing the "media" folder
         */
        explicit Engine(const std::filesystem::path &baseMediapath = ".");
        Engine(const Engine&) = delete;
        ~Engine();

        /**
         * \brief Runs the engine
         * 
         * Only stops when the user requests game termination.
         */
        void run();

    private:
        Context m_context;

    private:
        void createWindow(const files::GameDescription::WindowDescription &window, const std::string &version);
    };
}