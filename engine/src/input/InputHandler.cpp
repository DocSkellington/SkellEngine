#include "SkellEngine/input/InputHandler.hpp"

#include "SkellEngine/Context.h"
#include "SkellEngine/utilities/json_fusion.h"

namespace engine::input {
    InputHandler::InputHandler(Context &context) :
        m_context(context) {

    }

    void InputHandler::addInput(const std::string &eventType, const sf::Event &input) {
        auto itr = m_inputMappings.find(eventType);

        if (itr == m_inputMappings.end()) {
            auto em = m_inputMappings.emplace(eventType, InputMapping());
            if (!em.second) {
                tmx::Logger::log("InputHandler: could not add an input for event type: " + eventType, tmx::Logger::Type::Warning);
                return;
            }
            else {
                itr = em.first;
                tmx::Logger::log("InputHandler: added a new event type: " + eventType);
            }
        }

        itr->second.addInput(input);
        tmx::Logger::log("InputHandler: added an input for event type: " + eventType);
    }

    void InputHandler::proccess(const sf::Event &event) const {
        for (const auto& itr : m_inputMappings) {
            auto res = itr.second.process(event);
            if (res.first) {
                m_context.eventHandler->sendEvent(itr.first, utilities::json_fusion(itr.second.getJSON(), res.second));
            }
        }
    }

    InputHandler::InputMapping::InputMapping() :
        m_json() {

    }

    InputHandler::InputMapping::InputMapping(const nlohmann::json &json) :
        m_json(json) {

    }

    void InputHandler::InputMapping::addInput(const sf::Event &event) {
        m_inputs.emplace_back(event);
    }

    std::pair<bool, nlohmann::json> InputHandler::InputMapping::process(const sf::Event &event) const {
        nlohmann::json json;
        for (const auto &input : m_inputs) {
            if (input.type == event.type) {
                switch(input.type) {
                // sf::Event.size
                case sf::Event::EventType::Resized:
                    json["width"] = event.size.width;
                    json["height"] = event.size.height;
                    break;
                // sf::Event.text
                // case sf::Event::EventType::TextEntered:
                //     return 
                // sf::Event without data
                case sf::Event::EventType::Closed:
                case sf::Event::EventType::LostFocus:
                case sf::Event::EventType::GainedFocus:
                default:
                    break;
                }

                return std::make_pair(true, json);
            }
        }
        return std::make_pair(false, json);
    }

    const nlohmann::json& InputHandler::InputMapping::getJSON() const {
        return m_json;
    }
}