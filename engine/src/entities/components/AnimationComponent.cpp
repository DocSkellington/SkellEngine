#include "SkellEngine/entities/components/AnimationComponent.h"

#include "SkellEngine/animations/ColorAnimation.h"

namespace engine::entities::components {
    AnimationComponent::AnimationComponent(states::StateContext &stateContext) :
        Component(stateContext),
        m_animator(m_animations) {
    }

    void AnimationComponent::create(const nlohmann::json &description) {
        // TODO: loop, repeat, etc.
        tmx::Logger::log("Animation component");
        if (description.is_object()) {
            for (const auto &[name, desc] : description.items()) { 
                std::cout << desc.dump(4) << "\n";
                if (auto type = desc.find("type") ; type != desc.end() && type->is_string()) {
                    sf::Time duration;
                    if (auto dur = desc.find("duration") ; dur != desc.end() && dur->is_number()) {
                        duration = sf::seconds(*dur);
                    }
                    else {
                        tmx::Logger::log("AnimationComponent: 'duration' field absent or has an invalid type (it should be a number). The length of the animation defaults to 1 second", tmx::Logger::Type::Warning);
                    }

                    if (*type == "frame") {
                        // createFrameAnimation(description);
                    }
                    else if (*type == "color") {
                        if (auto colors = desc.find("colors") ; colors != desc.end() && colors->is_object()) {
                            std::cout << colors->dump(4) << "\n";
                            m_animations.addAnimation(name, animations::ColorAnimation(*colors), duration);
                        }
                        else {
                            tmx::Logger::log("AnimationComponent: invalid color animation description: the field 'colors' must be present", tmx::Logger::Type::Warning);
                        }
                    }
                    else if (*type == "fade") {
                        // createColorAnimation(description);
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