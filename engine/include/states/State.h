#pragma once

namespace engine::states {
    /**
    * \brief A state
    * \todo all
    */
    class State {
    public:
        explicit State();
        virtual ~State();

        void onCreate();
        void onDestroy();

        void activate();
        void deactivate();
    };
}