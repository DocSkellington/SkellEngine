---
layout: tutorial
title: States
shorttitle: States
---

Games usually must behave differently if it's paused, running or if it has just opened.
For example, when the player launches the program, it's nice to show a menu to start a new game, load a save, open a configuration tool, and so on. 
Each of these different behaviours is typically implemented by a State.
In this tutorial, we explain how to use and create states.

For this tutorial, let's say we want to implement a state called `game`. 

# State manager
The [State Manager]({{ "/documentation/alpha/c++/classengine_1_1states_1_1StateManager.html" | relative_url }}) is the part of the engine that manages the different states.
The states are stored in a stack.

You can request to change the front state to another one and to remove a state from the stack.
However, the requests are only processed at the end of the game loop.

When the engine calls the update function of the state manager, the first state in the stack is updated.
Then, if the state is _transcendant_, the next state is updated.
The update routine keeps updating states until a non-transcendant one is reached or if every state was updated.
The same logic applies for input handling.

The draw function is almost identical to the update function.
The first state is considered.
If it is _transparent_, the next state is considered, and so on until a non-transparent is reachd or if every state was considered.
Then, the states are drawn starting from the state that is the **deepest** in the stack.
This allows to draw multiple states while still making sure that the front state is the one displayed above the second (and so on).

# How does a state work
First, the state is _created_ (duh).
When the state becomes the front state in the stack, it gets _activated_.
At the opposite, when the state moves from the front seat, it gets _deactivated_.

Each frame, the state can handle inputs (the default implementation gives the input to the GUI), get updated and drawn, if the state manager calls the respective function.

This section is quite vague but the exact behaviour of a state highly depends on the state implementation.

# State context
Each state has its own [State context]({{ "documentation/alpha/c++/structengine_1_1states_1_1StateContext.html" | relative_url }}).
This context stores:
  - A reference to the [global context](loop#context-and-initialisation).
  - The [Entity manager](entities).
  - The [System manager](systems).
  - The [Level](level).
  - The GUI.
  - The name of the state.

This means that each state has its own systems, entities, GUI and level.

The following tutorials will explain the different parts, except for the GUI (see the [TGUI documentation](https://tgui.eu)).
You can click on the links above to directly jump to the related page.

# Create a new state
The engine does not implement any default state.
Therefore, you always have to define at least one state to be able to actually start a game.

This state draws a button that prints "Hello, world!" in the console.
We also want our state to be transparent and transcendant.

You can implement a state in both C++ and Lua. 

## C++
Every state defined in C++ must inherit from the [engine::states::State]({{ "/documentation/alpha/c++/classengine_1_1states_1_1State.html" | relative_url}}) class and must define the pure virtual functions.
The state must also register itself in order to be recognised by the state manager.

```cpp
#include <SkellEngine/states/State.h>

class GameState : public engine::states::State {
public:
    GameState(engine::states::StateManager &manager, const std::string &name) :
        State(manager, name, true, true) {

    }
        /* alternatively:
        State(manager, name) {
            setIsTransparent(true);
            setIsTranscendant(true);
        }
        */

    void onCreate() override {
        tgui::Button button = tgui::Button::create("Press me");
        sf::Vector2u windowSize = getStateContext().context.window->getSize();
        button->setPosition(windowSize.x / 2, windowSize.y / 2);
        button->connect("pressed", onButtonPressed);
        getStateContext().gui->add(button);
    }

    void onDestroy() override {}

    void activate() override {}

    void deactivate() override {}

    void update(sf::Int64 deltatime) {}

    void draw(sf::RenderTarget &target) {
        getStateContext().gui->draw();
    }

private:
    void onButtonPressed() {
        std::cout << "Hello, world!\n";
    }

    // We register the state under the name "game"
    REGISTER_STATE(GameState, "game");
};
```

## Lua
Lua files implementing states must be placed in the states folder defined in the [Game description](FileManager#game-description).
The name of the state is the name of the file. So, for our `game` state, we create a `game.lua` file in the directory:

```lua
TODO: the Lua implementation
```