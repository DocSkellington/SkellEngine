#include "SkellEngine/animations/FrameAnimation.h"

#include "SkellEngine/animations/detail/to_from_sfml_color.h"

namespace engine::animations {
    FrameAnimation::FrameAnimation(const nlohmann::json &description) {
        if (description.is_array()) {
            for (const auto &frame : description) {
                addFrame(frame);
            }
        }
        else {
            throw std::invalid_argument("FrameAnimation: invalid frame animation description: the description must be a JSON array containing each frame");
        }
    }
    
    void FrameAnimation::addFrame(const nlohmann::json &description) {
        if (description.is_object()) {
            Frame frame;
            if (auto duration = description.find("duration") ; duration != description.end() && duration->is_number_float()) {
                frame.relativeDuration = duration->get<float>();
            }
            else {
                throw std::invalid_argument("FrameAnimation: invalid frame description: the field 'duration' is missing");
            }

            if (auto rect = description.find("rect") ; rect != description.end()) {
                if (rect->is_array() && rect->size() == 4) {
                    const nlohmann::json &rectDescription = rect->get<nlohmann::json>();

                    if (rectDescription[0].is_number_integer() && rectDescription[1].is_number_integer() && rectDescription[2].is_number_integer() && rectDescription[3].is_number_integer()) {
                        sf::IntRect frameRect;
                        frameRect.left = rectDescription[0].get<int>();
                        frameRect.top = rectDescription[1].get<int>();
                        frameRect.width = rectDescription[2].get<int>();
                        frameRect.height = rectDescription[3].get<int>();

                        frame.textureRect = frameRect;
                        frame.applyTextureRect = true;
                    }
                    else {
                        throw std::invalid_argument("FrameAnimation: invalid frame description: the 'rect' array can only contain integers");
                    }
                }
                else {
                    throw std::invalid_argument("FrameAnimation: invalid frame description: the 'rect' field must be an array of size 4");
                }
            }

            if (auto origin = description.find("origin") ; origin != description.end()) {
                if (origin->is_array() && origin->size() == 2) {
                    const nlohmann::json &originDescription = origin->get<nlohmann::json>();

                    if (originDescription[0].is_number_float() && originDescription[1].is_number_float()) {
                        sf::Vector2f origin(originDescription[0].get<float>(), originDescription[1].get<float>());
                        frame.origin = origin;
                        frame.applyOrigin = true;
                    }
                    else {
                        throw std::invalid_argument("FrameAnimation: invalid frame description: the 'origin' array can only contain floats");
                    }
                }
                else {
                    throw std::invalid_argument("FrameAnimation: invalid frame description: the 'origin' field must be an array of size 2");
                }
            }

            if (auto color = description.find("color") ; color != description.end()) {
                sf::Color c = detail::jsonToColor(color->get<nlohmann::json>());
                frame.color = c;
                frame.applyColor = true;
            }

            if (auto scale = description.find("scale") ; scale != description.end()) {
                if (scale->is_array() && scale->size() == 2) {
                    const nlohmann::json &scaleDescription = scale->get<nlohmann::json>();

                    if (scaleDescription[0].is_number_float() && scaleDescription[1].is_number_float()) {
                        sf::Vector2f scale(scaleDescription[0].get<float>(), scaleDescription[1].get<float>());
                        frame.scale.data = scale;
                        frame.scale.relative = false;
                        frame.applyScale = true;
                    }
                    else {
                        throw std::invalid_argument("FrameAnimation: invalid frame description: the 'scale' array can only contain floats");
                    }
                }
                else if (scale->is_object()) {
                    const nlohmann::json &scaleDescription = scale->get<nlohmann::json>();

                    if (auto s = scaleDescription.find("scale") ; s != scaleDescription.end()) {
                        const nlohmann::json &sD = s->get<nlohmann::json>();

                        if (sD[0].is_number_float() && sD[1].is_number_float()) {
                            sf::Vector2f scale(sD[0].get<float>(), sD[1].get<float>());
                            frame.scale.data = scale;
                            frame.scale.relative = false;
                            frame.applyScale = true;
                        }
                        else {
                            throw std::invalid_argument("FrameAnimation: invalid frame description: the 'scale' array can only contain floats");
                        }
                    }
                    else {
                        throw std::invalid_argument("FrameAnimation: invalid frame description: the 'scale' object must contain the object 'scale'");
                    }

                    if (auto relative = scaleDescription.find("relative") ; relative != scaleDescription.end() && relative->is_boolean()) {
                        frame.scale.relative = relative->get<bool>();
                    }
                }
                else {
                    throw std::invalid_argument("FrameAnimation: invalid frame description: the 'scale' field must be an array of size 2 or an object");
                }
            }

            if (auto rotation = description.find("rotation") ; rotation != description.end()) {
                if (rotation->is_number_float()) {
                    frame.rotation.data = rotation->get<float>();
                    frame.rotation.relative = false;
                    frame.applyRotation = true;
                }
                else if (rotation->is_object()) {
                    const nlohmann::json &rotationDescription = rotation->get<nlohmann::json>();

                    if (auto r = rotationDescription.find("rotation") ; r != rotationDescription.end() && r->is_number_float()) {
                        frame.rotation.data = r->get<float>();
                        frame.rotation.relative = false;
                        frame.applyRotation = true;
                    }
                    else {
                        throw std::invalid_argument("FrameAnimation: invalid frame description: the 'rotation' object must contain the float 'rotation'");
                    }

                    if (auto relative = rotationDescription.find("relative") ; relative != rotationDescription.end() && relative->is_boolean()) {
                        frame.rotation.relative = relative->get<bool>();
                    }
                }
                else {
                    throw std::invalid_argument("FrameAnimation: invalid frame description: the 'rotation' field must be a float or an object");
                }
            }

            m_frames.push_back(frame);
        }
        else {
            throw std::invalid_argument("FrameAnimation: invalid frame description: the description of a frame must be a JSON object");
        }
    }
}