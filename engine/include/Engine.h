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
        ~Engine();

        void run();
    private:
        Context context;
    };
}