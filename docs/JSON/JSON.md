JSON Format             {#json}
====
[TOC]

This document will only describe JSON fields the application can understand. For file hierarchy (where to use the JSON fields), see [the appropriate page](@ref file_hierarchy).

# Notation  {#json_notation}
Before starting to document, let me introduce a notation to ease your reading and my writing. A JSON field will be described by:

    `name`: type: description[: format]

where `type` is `string`, `integer`, `float`, `boolean`, `array` or `object`. `format` (which is optional) describes the way to write the information so that the application can correctly read it. A `$` (dollar symbol) followed by a variable name describes a variable that can be freely named.

Also, please see [w3schools](https://www.w3schools.com/js/js_json_syntax.asp) for more information on JSON syntax.

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
  - `map`: `string`: the name of the map to load; the map must be found in the folder defined in the game description (see [the appropriate documentation](@ref json_game_description)
  - `entities`: `object`: the list of entities as described [here](@ref json_entities)