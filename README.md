# TODO
* Context:
  * Pointers to:
    * sol2 state
    * RenderWindow
    * StateManager
    * TextureHolder
    * SystemManager
    * EntityManager
* Engine:
  * run
* StateManager:
  * switchTo
  * remove (and processRequests)
  * update
  * draw
  * handleEvent
  * State:
    * Default states with variable data + possibility of overriding (at least some functions)
    * OnCreate
    * OnDestroy
    * update
    * draw
    * handleEvent
    * activate
    * deactivate
    * transparency, transcendency
    * view
* SystemManager:
  * Render
  * Move
* Entities:
  * Components:
    * Graphical
    * Click
    * Speed
    * Position
* Maps:
  * Orthogonal
  * Isometric
* UI (with TGUI):
  * Buttons:
    * Radio
    * Normal
    * CheckBox
  * Labels
  * Sliders
  * Theme
* Shaders
* Particles