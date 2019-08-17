#include "SkellEngine/animations/FadeAnimation.h"

#include "SkellEngine/tmxlite/Log.hpp"

namespace engine::animations {
    FadeAnimation::FadeAnimation(float inRatio, float outRatio) :
        m_inRatio(inRatio),
        m_outRatio(outRatio) {
        checkRatio();
    }

    FadeAnimation::FadeAnimation(const nlohmann::json &description) {
        m_inRatio = m_outRatio = 0;
        if (auto in = description.find("in") ; in != description.end() && in->is_number()) {
            m_inRatio = *in;
        }
        else {
            tmx::Logger::log("FadeAnimation: the 'in' field in not present or is not a float. The value defaults to 0.", tmx::Logger::Type::Warning);
        }

        if (auto out = description.find("out") ; out != description.end() && out->is_number()) {
            m_outRatio = *out;
        }
        else {
            tmx::Logger::log("FadeAnimation: the 'out' field in not present or is not a float. The value defaults to 0.", tmx::Logger::Type::Warning);
        }

        checkRatio();
    }

    void FadeAnimation::checkRatio() const {
        if (!(0. <= m_inRatio && m_inRatio <= 1.)) {
            throw std::invalid_argument("FadeAnimation: the in ratio must be in [0, 1]");
        }
        if (!(0. <= m_outRatio && m_outRatio <= 1. - m_inRatio)) {
            throw std::invalid_argument("FadeAnimation: the out ratio must be in [0, 1 - inRatio]");
        }
    }
}