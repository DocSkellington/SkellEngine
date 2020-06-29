# SkellEngine
**SkellEngine** is an easy-to-use 2D game engine.

The full documentation is available [online](https://docskellington.github.io/SkellEngine/).

## Short description
**SkellEngine** allows to easily and quickly create a basic version of a game.
It simplifies the initialization of the application while allowing the user to implement any kind of game.
Indeed, thanks to its Entity-Component-System (ECS) approach, **SkellEngine** can be used for any kind of games.

## Features
  * States
    * You can define as many states as you want
  * Entity-Component-System
    * Entities simply contain components
  * C++ and Lua interfaces
    * States and systems can be defined in both languages
  * Logging (to console and/or to file)
  * Event system
    * Send an event that can be processed by listeners
  * Inputs
    * Associate a user input to an event (for instance, if the user pressed the space bar, an event can be sent)
  * 2D orthogonal graphics:
    * Tiled map (using TMX file format)
    * Sprites
    * Animations
      * Sprite sheet
      * Fade in/out
      * Color changes
  * Random number generator library
    * Ensures that Lua and C++ code use the same random generators
  * Graphical user interface (GUI) allowing to draw anything anywhere

## Planned features
The planned features are not given in a particular order.

  * Save system
  * Achievements
  * Physics engine
  * Shaders support
  * Easy conversion from world-coordinates to screen-coordinates

## Examples
Examples are provided in both C++ and Lua.

For the moment, there is only one example:
  * [Falling blocks](examples/FallingBlocks)