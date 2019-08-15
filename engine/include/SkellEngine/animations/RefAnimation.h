#pragma once

namespace engine::animations {
    template <typename Animation>
    class RefAnimation {
    public:
        explicit RefAnimation(Animation &animation) :
            m_animation(&animation) {
        }

        template<typename Animated>
        void operator()(Animated &animated, float progress) {
            if (m_animation) {
                (*m_animation)(animated, progress);
            }
        }

    private:
        Animation *m_animation;
    };
}