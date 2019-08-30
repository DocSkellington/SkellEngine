---
layout: tutorial
title: File Manager and Game description
shorttitle: File
---

The main purpose of the [File manager]({{ "/documentation/alpha/c++/classengine_1_1files_1_1FileManager.html" | relative_url }}) is to read the `game.json` found in the root of your media folder and to register the states and systems implemented in Lua.
You can define the path to the media folder when you create the [engine]({{ "/documentation/alpha/c++/classengine_1_1Engine.html#a3fd60aa34558edee7d2b6f658681ef53" | relative_url}}).

# Game description
The `game.json` is a JSON file that contains the configuration of the [Context]({{ "/documentation/alpha/c++/classengine_1_1Context.html" | relative_url }}).
Each part of the context is described by an object in the file.
The following table gives the supported fields.
If a row starts with a tabulation, it means the field must belong to the last defined object.
Every path must be relative to the root of the media folder.

| Name of the field             | Expected type       | Description                                                                                                                                     | Default value   |
|-------------------------------|---------------------|-------------------------------------------------------------------------------------------------------------------------------------------------|-----------------|
| name                          | String              | The name of the game                                                                                                                            | Empty string    |
|-------------------------------|---------------------|-------------------------------------------------------------------------------------------------------------------------------------------------|-----------------|
| version                       | String              | The version of the game. Recommended format: x.x.x                                                                                              | Empty string    |
|-------------------------------|---------------------|-------------------------------------------------------------------------------------------------------------------------------------------------|-----------------|
| log                           | Object              | The description of the logger                                                                                                                   | See the fields  |
| &emsp;file                    | String              | The path to the file where the logger will write                                                                                                | `log.txt`       |
| &emsp;output                  | String              | Defines where the logger prints the messages.<br/>Accepted values: `none`, `console`, `file` or `all`                                           | `none`          |
| &emsp;level                   | String              | Defines the minimum type the messages must have to be printed.<br/>Accepted values: `info`, `warning` or `error`                                | `info`          |
|-------------------------------|---------------------|-------------------------------------------------------------------------------------------------------------------------------------------------|-----------------|
| window                        | Object              | The description of the window                                                                                                                   | See the fields  |
| &emsp;fullscreen              | Boolean             | If true, the window is displayed fullscreen and ignores the values size, showTitlebar, showResize, showClose, showEngine and showVersion        | False           |
| &emsp;size                    | Array of 2 integers | The size of the window. The first value is the width and the second is the height                                                               | [800, 600]      |
| &emsp;showTitlebar            | Boolean             | If true, the titlebar of the window is displayed. If false, the values showResize, showClose, showEngine and showVersion are ignored            | True            |
| &emsp;showResize              | Boolean             | If true, the window can be resized and the resize button is shown in the titlebar                                                               | True            |
| &emsp;showClose               | Boolean             | If true, the close button is shown in the titlebar                                                                                              | True            |
| &emsp;showEngine              | Boolean             | If true, the engine name is shown in the titlebar                                                                                               | True            |
| &emsp;showVersion             | Boolean             | If true, the version (defined in the version field) of the game is shown in the titlebar                                                        | True            |
| &emsp;title                   | String              | The string to show in the titlebar                                                                                                              | Empty string    |
| &emsp;antialiasingLevel       | Integer             | The level of the antialiasing level                                                                                                             | 0               |
| &emsp;FPS                     | Integer             | The FPS limit. If 0 or a negative number, the FPS is not limited                                                                                | 60              |
| &emsp;verticalSynchronisation | Boolean             | If true, the vertical synchronisation is set and the FPS limit is ignored                                                                       | False           |
|-------------------------------|---------------------|-------------------------------------------------------------------------------------------------------------------------------------------------|-----------------|
| media                         | Object              | The description of the media folder                                                                                                             | See the fields  |
| &emsp;baseSprites             | String              | The path to the folder containing the spritesheets                                                                                              | `sprites/`      |
| &emsp;systems                 | String              | The path to the folder containing the systems defined in Lua                                                                                    | `systems/`      |
| &emsp;states                  | String              | The path to the folder containing the states defined in Lua                                                                                     | `states/`       |
| &emsp;fonts                   | String              | The path to the folder containing the font files                                                                                                | `fonts/`        |
| &emsp;entities                | String              | The path to the folder containing the global entities definitions                                                                               | `entities/`     |
| &emsp;levels                  | String              | The path to the folder containing the levels descriptions                                                                                       | `levels/`       |
| &emsp;maps                    | String              | The path to the folder containing the `*.tmx` files                                                                                             | `maps/`         |
| &emsp;input                   | String              | The path to the JSON file describing the input-event bindings                                                                                   | `keys.json`     |
|-------------------------------|---------------------|-------------------------------------------------------------------------------------------------------------------------------------------------|-----------------|
| states                        | Object              | The description of the states                                                                                                                   | See the fields  |
| &emsp;first                   | String              | The state to launch when the engine starts                                                                                                      | `intro`         |
|-------------------------------|---------------------|-------------------------------------------------------------------------------------------------------------------------------------------------|-----------------|

Examples can be seen in the [Games from scratch tutorials]({{ "/tutorials/index.html#games-from-scratch" | relative_url }})

# File manager
As stated above, the file manager reads the `game.json` and extracts the configurations.
If something is invalid, a message will be printed to the console (ignoring the logger configuration).
The configurations are then used by the context to construct the other objects.

The file manager can also be used to load the states and systems defined in Lua and the fonts and to load and use textures and fonts.