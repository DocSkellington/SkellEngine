#pragma once

#include "Context.h"
#include "files/GameDescription.h"

/**
 * \defgroup Engine Engine
 * 
 * The C++ functions
 */

/**
* \brief This numespace contains everything the engine needs
*/
namespace engine {
    /**
    * \brief The engine
    * \ingroup Engine
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
        void createWindow(const files::GameDescription::WindowDescription &window, const std::string &version);
    };
}