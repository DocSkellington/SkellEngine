#include "SkellEngine/animations/ColorAnimation.h"

#include "SkellEngine/animations/detail/to_from_sfml_color.h"

#include <list>

namespace engine::animations {
    ColorAnimation::ColorAnimation(const thor::ColorGradient &gradient)  :
        m_gradient(gradient) {

    }

    ColorAnimation::ColorAnimation(const nlohmann::json &description) {
        if (description.is_object()) {
            if (auto first = description.find("first") ; first != description.end()) {
                m_gradient[0] = detail::jsonToColor(*first);
            }
            else {
                throw std::invalid_argument("ColorAnimation: invalid description: the field 'first' must be present if 'colors' is an object");
            }

            if (auto last = description.find("last") ; last != description.end()) {
                m_gradient[1] = detail::jsonToColor(*last);
            }
            else {
                throw std::invalid_argument("ColorAnimation: invalid description: the field 'last' must be present if 'colors' is an object");
            }
        }
        else if (description.is_array()) {
            std::list<nlohmann::json> notObjects;
            std::list<nlohmann::json> objects;
            for (auto inArray : description) {
                if (inArray.is_object()) {
                    objects.push_back(inArray);
                }
                else {
                    notObjects.push_back(inArray);
                }
            }

            if (notObjects.size() < 2) {
                throw std::invalid_argument("ColorAnimation: invalid description: the array describing the color must have at least 2 non-object values");
            }

            float i = 1;
            for (auto itr = notObjects.begin() ; itr != notObjects.end() ; ++itr) {
                float position = 0;
                if (itr == notObjects.begin()) {
                    position = 0;
                }
                else if (itr == std::prev(notObjects.end())) {
                    position = 1;
                }
                else {
                    // notObjects.size() - 1: if we have the array ["red", "blue", "black"], we want "blue" to be in position 0.5
                    position = i++ / (notObjects.size() - 1);
                }
                m_gradient[position] = detail::jsonToColor(*itr);
            }

            for (const auto &object : objects) {
                float i = 0;
                if (auto position = object.find("position") ; position != object.end() && position->is_number() && 0 <= *position && *position <= 1) {
                    i = *position;
                }
                else {
                    throw std::invalid_argument("ColorAnimation: invalid description: an object in an array must define the field 'position' and it must be a number in [0,1]");
                }

                if (auto color = object.find("color") ; color != object.end()) {
                    m_gradient[i] = detail::jsonToColor(*color);
                }
                else {
                    throw std::invalid_argument("ColorAnimation: invalid description: the field 'color' must be present in an object in the array 'colors'");
                }
            }
        }
        else {
            throw std::invalid_argument("ColorAnimation: invalid description: the description must be an array or an object");
        }
    }
}