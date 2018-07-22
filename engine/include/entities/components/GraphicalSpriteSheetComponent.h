#pragma once

#include <SFML/Graphics.hpp>

#include "entities/components/Component.h"
#include "Context.h"

namespace engine::entities::components {
    /**
     * \brief Describes the sprite sheet of a component
     * \todo Animations
     */
    class GraphicalSpriteSheetComponent : public Component {
    public:
        explicit GraphicalSpriteSheetComponent();
        GraphicalSpriteSheetComponent(const GraphicalSpriteSheetComponent&) = delete;
        virtual ~GraphicalSpriteSheetComponent();

        virtual void create(const nlohmann::json &jsonTable) override;

        /**
         * \brief Returns a const reference to the sprite
         */
        const sf::Sprite &getSprite() const;

        virtual void set(const std::string &name, long value) override;
        virtual void set(const std::string &name, double value) override;
        virtual void set(const std::string &name, bool value) override;
        virtual void set(const std::string &name, const std::string &value) override;

        virtual void set(const std::string &name, sol::nil_t value) override;
        virtual void set(const std::string &name, const sol::table &value) override;

        virtual void set(const std::string &name, nlohmann::json value) override;

        virtual std::pair<long, bool> getInt(const std::string &name) override;
        virtual std::pair<double, bool> getFloat(const std::string &name) override;
        virtual std::pair<bool, bool> getBool(const std::string &name) override;
        virtual std::pair<std::string, bool> getString(const std::string &name) override;
        virtual std::pair<sol::object, bool> getObject(const std::string &name) override;

    private:
        sf::Sprite m_sprite;

        static Component::RegisterComponent<GraphicalSpriteSheetComponent> rcpc;
    };
}