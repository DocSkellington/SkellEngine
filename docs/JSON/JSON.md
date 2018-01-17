JSON Format             {#json}
====
[TOC]

This document will only describe JSON fields the application can understand. For file hierarchy (where to use the JSON fields), see [the appropriate page](@ref file_hierarchy).

# Notation  {#json_notation}
Before starting to document, let me introduce a notation to ease your reading and my writing. A JSON field will be described by:

    `name`: type: description[: format]

where `type` is `string`, `integer`, `float`, `boolean`, `array` or `object`. `format` (which is optional) describes the way to write the information so that the application can correctly read it. A `$` (dollar symbol) followed by a variable name describes a variable that can be freely named.

Also, please see [w3schools](https://www.w3schools.com/js/js_json_syntax.asp) for more information on JSON syntax.

# Game {#json_game}
The `game.json` file supports the following field:
  - `game`: `object`: describes the game's description
    - `name`: `string`: the name of the game
    - `version`: `string`: the version of the game: x.x.x (where each x is an integer) is recommend but not mandatory
    - `firstState`: `string`: the state the game must start in: the name of a known state (see [states](@ref json_states))
  - `log`: `object`: describes the log system
    - `output`: `string`: where the logger must write: `console` will write in the console/commad prompt, `file` will write in the `media/log.txt` file and `all` will write in both the console and file
  - `media`: `object`: the global media's settings
    - `baseSprites`: `string`: the path to the root of the sprites folder
    - `maps`: `string` or `object` : the path to the folder containing the tiled maps or an object containing:
      - `tilesets`: `string`: the path to the folder containing the tilesets files; if not defined, same as `maps`
      - `images`: `string`: the path to the folder containing the images used by the maps; if not defined, same as `maps`
      - `maps`: `string`: the path to the folder conainting the tiled maps
  - `window`: `object`: the graphical window
    - `fullscreen`: `boolean`: whether the window must be fullscreen or not; the window's size is always the same as the desktop's size
    - `size`: `array`: the width and height of the window
    - `antialiasingLevel`: `int`: the level of anti-aliasing
    - `showTitlebar`: `boolean`: whether to show the titlebar or not; overrides `showResize` and `showClose`
    - `showResize`: `boolean`: whether to allow the user to resize the window or not
    - `showClose`: `boolean`: whether to show the close button or not
    - `title`: `string`: the string to print in the window border
    - `showVersion`: `boolean`: whether to show the game's version in the window border or not
    - `showEngine`: `boolean`: whether to show engine's information or not
  - `systems`: `arrays` of `strings`: each string is the name of a system to use in the game (see [systems](@ref json_systems))

# Entities {#json_entities}
Each entity must be described as following:
  - `type`: `string`: the type of the entity
  - `data`: `object`: the components (see [components](@ref json_components))

## Components {#json_components}
Each component has a unique description. Please, see @subpage json_full_components for the full list.

# States {#json_states}
# Systems {#json_systems}

# Levels {#json_levels}
Each level must be described as following:
  - `map`: `string`: the name of the map to load; the map must be found in the folder defined in the game description (see [the appropriate documentation](@ref json_game)
  - `entities`: `object`: the list of entities as described [here](@ref json_entities)