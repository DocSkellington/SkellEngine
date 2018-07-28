Game Description      {#json_game_description}
===
[TOC]

The game description file must be named `game.json` and must be in the root of `media`.

# Accepted fields     {#json_game_description_fields}

| Name of the field         | Expected type       | Description                                                                                                         | Default value  |
|-------------------        |---------------      |-----------------------------------------------------                                                                |----------------|
| `name`                    | String              | The name of your game.                                                                                              | Empty string   |
| `version`                 | String              | The version of your game. Recommend format: "x.x.x"                                                                 | Empty string   |
| `log`                     | Object              | Describes how to use the logger.                                                                                    | See the fields |
|  &emsp;`output`           | String              | Defines where to write the logger output.<br/>Accepted values : `console`, `file`, `all`, `none`                | `none`         |
| `window`                  | Object              | Defines the window.                                                                                                 | See the fields |
| &emsp;`fullscreen`        | Boolean             | Whether the window must be fullscreen or not.<br/>If it set to `true`, `size` is ignored.                          | `false`        |
| &emsp;`size`              | Array of 2 integers | The width and the height (in this order) of the window.<br/>If `fullscreen` is set to `true`, the field is ignored. | [800, 600]     |
| &emsp;`showTitlebar`      | Boolean             | Whether to show the titlebar or not.                                                                                | `true`         |
| &emsp;`showResize`        | Boolean             | Whether to show the resize button and to enable resizing of the window or not.                                      | `true`         |
| &emsp;`showClose`         | Boolean             | Whether to show the close button in the titlebar or not.                                                            | `true`         |
| &emsp;`showEngine`        | Boolean             | Whether to show the engine name in the titlebar or not.                                                             | `true`         |
| &emsp;`showVersion`       | Boolean             | Whether to show the game's version in the titlebar or not. See the `version` field.                               | `true`         |
| &emsp;`title`             | String              | The string to show in the titlebar.                                                                                 | Empty string   |
| &emsp;`antialiasingLevel` | Integer             | The level of the antialiasing.                                                                                      | Empty string   |
| `media`                   | Object              | Describes the folders to use. The pathes are relative to the `media` root.                                          | See the fields |
| &emsp;`baseSprites`       | String              | The base folder for sprites. Every sprite must be in this folder.                                                   | `media/sprites`|
| &emsp;`maps`              | String              | The base folder for maps. Every map file must be in this folder.                                                    | `media/maps`   |
| &emsp;`systems`           | String              | The folder that contains every . Every map file must be in this folder.                                             | `media/systems`|
| `states`                  | Object              | Describes the states.                                                                                               | See the fields |
| &emsp;`first`             | String              | The name of the first state to launch at game startup.                                                              | `intro`    |

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
        "baseSprites": "media/sprites/path",
        "maps": "media/path/to/maps",
        "systems": "media/path/to/systems"
      },
      "states" : {
        "first": "MainMenu"
      }
    }