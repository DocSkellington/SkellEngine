#include "SkellEngine/states/State.h"

class IntroState : public engine::states::State {
public:
    IntroState(engine::states::StateManager &manager, const std::string &name) :
        State(manager, name, false, false) {

    }

    virtual void onCreate() override {

    }

    virtual void onDestroy() override {
        
    }

    virtual void activate() override {
        
    }

    virtual void deactivate() override {
        
    }

    virtual void update(sf::Int64 deltaTime) override {

    }

    virtual void draw(sf::RenderTarget &target) override {

    }

private:
    void nextLogo(const engine::events::Event &event) {
        
    }

    REGISTER_STATE(IntroState, "intro")
};