#pragma once

#include "Context.h"

/**
* \brief This numespace contains everything the engine needs
*/
namespace engine {
    /**
    * \brief The engine
    */
    class Engine final {
    public:
        explicit Engine();
        Engine(const Engine&) = delete;
        ~Engine();

        void run();

    private:
        Context m_context;

    private:
        void createWindow(const nlohmann::json &window, const std::string &version);
    };
}