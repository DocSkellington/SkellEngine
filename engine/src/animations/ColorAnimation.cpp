#include "SkellEngine/animations/ColorAnimation.h"

#include "SkellEngine/tmxlite/Log.hpp"

#include "SkellEngine/animations/detail/to_from_sfml_color.h"

namespace engine::animations {
    ColorAnimation::ColorAnimation(const thor::ColorGradient &gradient)  :
        m_gradient(gradient) {

    }

    ColorAnimation::ColorAnimation(const nlohmann::json &description) {
        if (description.is_object()) {
            if (auto first = description.find("first") ; first != description.end()) {
                if (first->is_string()) {
                    m_gradient[0] = detail::stringToColor(*first);
                }
            }
            else {
                tmx::Logger::log("ColorAnimation: invalid description: the field 'first' must be present if 'colors' is an object", tmx::Logger::Type::Warning);
            }

            if (auto last = description.find("last") ; last != description.end()) {
                if (last->is_string()) {
                    m_gradient[0] = detail::stringToColor(*last);
                }
            }
            else {
                tmx::Logger::log("ColorAnimation: invalid description: the field 'first' must be present if 'colors' is an object", tmx::Logger::Type::Warning);
            }
        }

    }
}