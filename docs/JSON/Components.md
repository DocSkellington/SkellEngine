JSON Components         {#json_full_components}
======
[TOC]

This document describes the components the engine has by default.

Each section will present the needed fields for each component. See [JSON page](@ref json)

# Position {#json_full_position}
  - `position`: `array` of `integers`: the x and y position

# SpriteSheet {#json_full_spritesheet}
  - `spritesheet`: `object`: the sprite sheet description
    - `filePath`: `string`: the path to the texture to use (from the base sprites folder)

# Extern {#json_full_extern}
As a component is only made to contain data (not to actively use them), an extern component can contain anything you want.

  - `#name`: `object`: your component; you can put anything in the object

\todo check extern JSON type