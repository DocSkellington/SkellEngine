Game Description      {#json_game_description}
===
[TOC]

The game description file must be named `game.json` and must be in the root of `media`.

# Accepted fields     {#json_game_description_fields}
If a _name of the field_ starts with a tabulation, it means the field belongs to the last defined _object_.

| Name of the field         | Expected type       | Description                                                                                                         | Default value  |
|-------------------        |---------------      |-----------------------------------------------------                                                                |----------------|
| `name`                    | String              | The name of your game.                                                                                              | Empty string   |
| `version`                 | String              | The version of your game. Recommended format: "x.x.x"                                                               | Empty string   |
| `log`                     | Object              | Describes how to use the logger.                                                                                    | See the fields |
|  &emsp;`output`           | String              | Defines where to write the logger output.<br/>Accepted values : `console`, `file`, `all`, `none`                    | `none`         |
| `window`                  | Object              | Defines the window.                                                                                                 | See the fields |
| &emsp;`fullscreen`        | Boolean             | Whether the window must be fullscreen or not.<br/>If it set to `true`, `size` is ignored.                           | `false`        |
| &emsp;`size`              | Array of 2 integers | The width and the height (in this order) of the window.<br/>If `fullscreen` is set to `true`, the field is ignored. | [800, 600]     |
| &emsp;`showTitlebar`      | Boolean             | Whether to show the titlebar or not.                                                                                | `true`         |
| &emsp;`showResize`        | Boolean             | Whether to show the resize button and to enable resizing of the window or not.                                      | `true`         |
| &emsp;`showClose`         | Boolean             | Whether to show the close button in the titlebar or not.                                                            | `true`         |
| &emsp;`showEngine`        | Boolean             | Whether to show the engine name in the titlebar or not.                                                             | `true`         |
| &emsp;`showVersion`       | Boolean             | Whether to show the game's version in the titlebar or not. See the `version` field.                                 | `true`         |
| &emsp;`title`             | String              | The string to show in the titlebar.                                                                                 | Empty string   |
| &emsp;`antialiasingLevel` | Integer             | The level of the antialiasing.                                                                                      | Empty string   |
| &emsp;`FPS`               | Integer             | The frame per second limit. If 0 or a negative number, there is no limit.                                           | 60             |
| &emsp;`verticalSynchronisation` | Boolean       | Whether to enable the vertical synchronisation or not. If true, the FPS limit is ignored.                          | `false`        |
| `media`                   | Object              | Describes the folders to use. The pathes are relative to the `media` root.                                          | See the fields |
| &emsp;`baseSprites`       | String              | The base folder for sprites. Every sprite must be in this folder.                                                   | `sprites`      |
| &emsp;`maps`              | String              | The base folder for maps. Every map file must be in this folder.                                                    | `maps`         |
| &emsp;`systems`           | String              | The folder that contains every system. Every Lua system file must be in this folder.                                | `systems`      |
| &emsp;`fonts`             | String              | The folder that contains the font files. Every font file must be in this folder.                                    | `fonts`        |
| &emsp;`entities`          | String              | The base folder for entities. Every entity file must be in this folder.                                             | `entities`     |
| &emsp;`states`            | String              | The base folder for states. Every state file must be in this folder.                                                | `states`       |
| &emsp;`levels`            | String              | The base folder for levels. Every level file must be in this folder.                                                | `levels`       |
| &emsp;`input`             | String              | The path to the JSON file describing the mappings between user inputs and events sent in the engine.                | `keys.json`    |
| `states`                  | Object              | Describes the states.                                                                                               | See the fields |
| &emsp;`first`             | String              | The name of the first state to launch at game startup.                                                              | `intro`        |

# Example               {#json_game_description_example}
Before your amazed eyes, let me show you an example of a `game.json` file.

    {
      "name": "Game",
      "version": "1.0.0",
      "log" : {
        "output": "all"
      },
      "window" : {
        "fullscreen": false,
        "size": [1920, 1080],
        "title": "Game title",
        "showTitlebar": true,
        "showResize": false,
        "showClose": false,
        "showVersion": true,
        "showEngine": false,
        "antialiasingLevel": 8
      },
      "media" : {
        "baseSprites": "sprites/path",
        "maps": "path/to/maps",
        "systems": "path/to/systems",
        "entities": "path/to/entities",
        "fonts": "path/to/fonts",
        "states": "path/to/states",
        "levels": "path/to/levels",
        "input": "path/to/keys.json"
      },
      "states" : {
        "first": "MainMenu"
      }
    }