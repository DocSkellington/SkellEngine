# TODO
* StateManager:
  * handleEvent
  * State:
    * Default states with variable data + possibility of overriding (at least some functions)
    * view
* SystemManager:
  * Render:
    * Orthogonal
    * Isometric
  * Move
* Entities:
  * Components:
    * Graphical:
      * SpriteSheet:
        * Animations
    * Click
    * Speed
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
* Events:
  * A system must be able to indicate which event it wants (to filter)
  * Message:
    * Sender
    * Type
    * Info
  * Call directly the correct methods and let the classes create their own queue, if needed? Example: play a sound, move a character, tutorial help, dialogue, save, ...
    * For input, it needs callbacks
* Lua:
  * Names:
    * `entities/entityType`: default values
    * `entities/entityType/entityName`: specific instance
    * `systems/systemName`
    * `states/stateName`
* Log:
  * Different levels:
    * Info
    * Warning
    * Error
  * Configuration in game.json
