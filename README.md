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
  * CollisionDetection
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
* Map:
  * Switch to tmxlite
    * Include sources in project
  * Draw and test physics only on visible parts:
    * On creation:
      * Load every layer:
        * Separate Tile, Object and Image
    * On update:
      * Get camera information
      * Compute tiles (upper left + width + height) to be rendered
    * On draw:
      * Render
  * System? Map on its own (with render, update, collisions' tester, ...)? Map on its own with utilities to retrieve the tiles seen (for graphics) and check if a position is inside an object (+ return type and properties if yes)?
* Documentation:
  * Update Dependencies and Building
  * Add default values