#include "SkellEngine/animations/FadeAnimation.h"

#include "SkellEngine/errors/InvalidJSON.h"

namespace engine::animations {
    FadeAnimation::FadeAnimation(float inRatio, float outRatio) :
        m_inRatio(inRatio),
        m_outRatio(outRatio) {
        checkRatio();
    }

    FadeAnimation::FadeAnimation(const nlohmann::json &description) {
        m_inRatio = m_outRatio = 0;
        if (auto in = description.find("in") ; in != description.end()) {
            if (in->is_number_float()) {
                m_inRatio = in->get<float>();
            }
            else {
                throw errors::InvalidJSON("FadeAnimation: the 'in' field must be a float");
            }
        }

        if (auto out = description.find("out") ; out != description.end()) {
            if (out->is_number()) {
                m_outRatio = *out;
            }
            else {
                throw errors::InvalidJSON("FadeAnimation: the 'out' field must be a float");
            }
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