#include "SkellEngine/entities/components/AnimationComponent.h"

#include "SkellEngine/animations/ColorAnimation.h"
#include "SkellEngine/animations/FadeAnimation.h"
#include "SkellEngine/animations/FrameAnimation.h"

namespace engine::entities::components {
    AnimationComponent::AnimationComponent(states::StateContext &stateContext) :
        Component(stateContext),
        m_animator() {
    }

    void AnimationComponent::create(const nlohmann::json &description) {
        if (description.is_object()) {
            for (const auto &[name, desc] : description.items()) { 
                if (auto type = desc.find("type") ; type != desc.end() && type->is_string()) {
                    sf::Time duration = sf::seconds(1);
                    bool loop = false;
                    unsigned int repeats = 1;

                    if (auto dur = desc.find("duration") ; dur != desc.end() && dur->is_number()) {
                        duration = sf::seconds(*dur);
                    }
                    else {
                        tmx::Logger::log("AnimationComponent: 'duration' field absent or has an invalid type (it should be a number). The length of the animation defaults to 1 second", tmx::Logger::Type::Warning);
                    }

                    if (auto l = desc.find("loop") ; l != desc.end() && l->is_boolean()) {
                        loop = l->get<bool>();
                    }
                    else {
                        tmx::Logger::log("AnimationComponent: 'loop' field absent or has an invalid type (it should be a boolean). It defaults to false");
                    }

                    if (auto r = desc.find("repeats") ; r != desc.end() && r->is_number()) {
                        repeats = r->get<unsigned int>();
                    }
                    else {
                        tmx::Logger::log("AnimationComponent: 'repeats' field absent or has an invalid type (it should be a number). It defaults to 1");
                    }

                    if (*type == "frame") {
                        if (auto frames = desc.find("frames") ; frames != desc.end()) {
                            try {
                                m_animations.addAnimation(name, animations::FrameAnimation(frames->get<nlohmann::json>()), duration, loop, repeats);
                            }
                            catch (const std::invalid_argument &e) {
                                tmx::Logger::logError("AnimationComponent: an error occured during the creation of a FrameAnimation:", e);
                            }
                        }
                        else {
                            tmx::Logger::log("AnimationComponent: invalid frame animation description: the field 'frames' must be present", tmx::Logger::Type::Warning);
                        }
                    }
                    else if (*type == "color") {
                        if (auto colors = desc.find("colors") ; colors != desc.end()) {
                            try {
                                m_animations.addAnimation(name, animations::ColorAnimation(*colors), duration, loop, repeats);
                            }
                            catch (const std::invalid_argument &e) {
                                tmx::Logger::logError("AnimationComponent: an error occured during the creation of a ColorAnimation:", e);
                            }
                        }
                        else {
                            tmx::Logger::log("AnimationComponent: invalid color animation description: the field 'colors' must be present", tmx::Logger::Type::Warning);
                        }
                    }
                    else if (*type == "fade") {
                        try {
                            m_animations.addAnimation(name, animations::FadeAnimation(desc), duration, loop, repeats);
                        }
                        catch (const std::invalid_argument &e) {
                            tmx::Logger::logError("AnimationComponent: an error occured during the creation of a FadeAnimation: ", e);
                        }
                    }
                    else {
                        tmx::Logger::log("AnimationComponent: invalid animation description: the type of the animation is unkown. It must be 'frame', 'color' or 'fade'", tmx::Logger::Type::Warning);
                    }
                }
                else {
                    tmx::Logger::log("AnimationComponent: the animation description must have a 'type' field and it must be a string. The animation will not be loaded.", tmx::Logger::Type::Warning);
                }
            }
        }
        else {
            tmx::Logger::log("AnimationComponent: the animation description must be an object with pairs (name of the animation, description of the animation)", tmx::Logger::Type::Warning);
        }
    }

    AnimationComponent::AnimationMap& AnimationComponent::getAnimationMap() {
        return m_animations;
    }

    const AnimationComponent::AnimationMap& AnimationComponent::getAnimationMap() const {
        return m_animations;
    }

    AnimationComponent::Animator& AnimationComponent::getAnimator() {
        return m_animator;
    }

    const AnimationComponent::Animator &AnimationComponent::getAnimator() const {
        return m_animator;
    }
}