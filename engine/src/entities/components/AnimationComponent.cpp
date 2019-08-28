#include "SkellEngine/entities/components/AnimationComponent.h"

#include "SkellEngine/animations/ColorAnimation.h"
#include "SkellEngine/animations/FadeAnimation.h"
#include "SkellEngine/animations/FrameAnimation.h"
#include "SkellEngine/errors/InvalidJSON.h"

namespace engine::entities::components {
    AnimationComponent::AnimationComponent(states::StateContext &stateContext) :
        MemberStorage(stateContext.context),
        Component(stateContext),
        m_animator() {
    }

    void AnimationComponent::create(const nlohmann::json &description) {
        if (description.is_object()) {
            std::string defaultAnimation = "";
            for (const auto &[name, desc] : description.items()) { 
                if (name == "default") {
                    if (desc.is_string()) {
                        defaultAnimation = desc.get<std::string>();
                    }
                    else {
                        getContext().logger.log("AnimationComponent: the 'default' field must be a string", LogType::Warning);
                    }
                }
                else {
                    if (auto type = desc.find("type") ; type != desc.end() && type->is_string()) {
                        sf::Time duration = sf::seconds(1);
                        bool loop = false;
                        unsigned int repeats = 1;
                        std::string eventToSend = "";

                        if (auto dur = desc.find("duration") ; dur != desc.end() && dur->is_number_float()) {
                            duration = sf::seconds(dur->get<float>());
                        }
                        else {
                            throw errors::InvalidJSON("AnimationComponent: 'duration' field absent or has an invalid type (it should be a float)");
                        }

                        if (auto toSend = desc.find("event") ; toSend != desc.end()) {
                            if (toSend->is_string()) {
                                eventToSend = toSend->get<std::string>();
                            }
                            else {
                                getContext().logger.log("AnimationComponent: 'event' must be a string. It defaults to an empty string (no event)", LogType::Warning);
                            }
                        }

                        if (auto l = desc.find("loop") ; l != desc.end()) {
                            if (l->is_boolean()) {
                                loop = l->get<bool>();
                            }
                            else {
                                getContext().logger.log("AnimationComponent: 'loop' must be a boolean. It defaults to false", LogType::Warning);
                            }
                        }

                        if (auto r = desc.find("repeats") ; r != desc.end()) {
                            if (r->is_number()) {
                                repeats = r->get<unsigned int>();
                            }
                            else {
                                getContext().logger.log("AnimationComponent: 'repeats' must be a number. It defaults to 1", LogType::Warning);
                            }
                        }

                        if (*type == "frame") {
                            if (auto frames = desc.find("frames") ; frames != desc.end()) {
                                try {
                                    m_animations.addAnimation(name, animations::FrameAnimation(frames->get<nlohmann::json>()), duration, eventToSend, loop, repeats);
                                }
                                catch (const std::exception &e) {
                                    getContext().logger.logError("AnimationComponent: an error occured during the creation of a FrameAnimation:", e);
                                }
                            }
                            else {
                                getContext().logger.log("AnimationComponent: invalid frame animation description: the field 'frames' must be present", LogType::Warning);
                            }
                        }
                        else if (*type == "color") {
                            if (auto colors = desc.find("colors") ; colors != desc.end()) {
                                try {
                                    m_animations.addAnimation(name, animations::ColorAnimation(*colors), duration, eventToSend, loop, repeats);
                                }
                                catch (const std::exception &e) {
                                    getContext().logger.logError("AnimationComponent: an error occured during the creation of a ColorAnimation:", e);
                                }
                            }
                            else {
                                getContext().logger.log("AnimationComponent: invalid color animation description: the field 'colors' must be present", LogType::Warning);
                            }
                        }
                        else if (*type == "fade") {
                            try {
                                m_animations.addAnimation(name, animations::FadeAnimation(desc), duration, eventToSend, loop, repeats);
                            }
                            catch (const std::exception &e) {
                                getContext().logger.logError("AnimationComponent: an error occured during the creation of a FadeAnimation: ", e);
                            }
                        }
                        else {
                            getContext().logger.log("AnimationComponent: invalid animation description: the type of the animation is unkown. It must be 'frame', 'color' or 'fade'", LogType::Warning);
                        }
                    }
                    else {
                        getContext().logger.log("AnimationComponent: the animation description must have a 'type' field and it must be a string. The animation will not be loaded.", LogType::Warning);
                    }
                }
            }

            // We apply the default animation
            if (defaultAnimation != "") {
                if (m_animations.hasAnimation(defaultAnimation)) {
                    m_animator.setDefault(m_animations.getAnimation(defaultAnimation));
                    m_animator.enableDefault();
                }
                else {
                    getContext().logger.log("AnimationComponent: invalid default animation: unknown animation", LogType::Warning);
                }
            }
        }
        else {
            getContext().logger.log("AnimationComponent: the animation description must be an object with pairs (name of the animation, description of the animation)", LogType::Warning);
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