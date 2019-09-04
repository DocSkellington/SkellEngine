---
layout: tutorial
title: Input handler and inputs
shorttitle: Inputs
---

The users interact with the game by pressing keyboard buttons, moving the mouse, ...
These interactions are called inputs in SkellEngine.

{% include important.html content="
Events and inputs are two different parts of the engine.
Events are used to transfer information <em>within</em> the engine while inputs are used to transfer information <em>from</em> the player <em>to</em> the engine.
" %}

# Input
An input is described by:
  1. The type of the event to send when the input is active.
  2. The type of the input (what the user must do to trigger the input).
  3. Whether a (or multiple) modifier(s) must be hold.
  4. An optional JSON payload to send with the event.
  5. The state in which the input is active.

Internally, inputs rely on the [SFML events](https://www.sfml-dev.org/tutorials/2.5/window-events.php).
The linked SFML documentation page describes the different input types.

`KeyPressed`, `MouseButtonPressed` and `JoystickButtonPressed` send only one event (at the frame the button is pressed).
If you'd rather have a new event sent each frame, use `KeyHold`, `MouseButtonHold` and `JoystickButtonHold`.

Modifiers are `control`, `left` and `alt` buttons.
Left and right modifiers are considered as different buttons.

## Input description
Let's take a look at the following description of one input:

```json
"eventType" : {
  "type": "Closed",
  "control": true,
  "lshift": true,
  "ralt": true,
  "state": "mainmenu",
  "key": "Up",
  "payload": "message"
}
```

This input is active when the user asks to close the window (via the close button in the titlebar).
The left shift (`lshift`) button must be hold when the close button is pressed, as well as the right alt (`ralt`) button.
`control` is equivalent to `lcontrol`.
So, the left control must also be hold.
The input is active only in the `mainmenu` game (if the window is closed in a different state, this input will not be active).
When the input is active, an event of type `eventType` is sent.

Every field not used by the input is considered as payload.
So, here, `"key": "Up"` and `"payload": "message"` are payload and are added in the event sent when the input is active.
Note that `key` is used by keyboard key inputs (as explained below) but not by `Closed` inputs.
Therefore, in this case, `key` is taken as a payload.

### Key inputs
For keyboard key inputs, you must precise the key that must be pressed/released/hold by adding a `key` field in the description.
See [this SFML enumeration](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Keyboard.php#acb4cacd7cc5802dec45724cf3314a142) for the available keys.
You can also use `Any` to trigger the input when any key is pressed.
If the `key` field is missing, `Any` is used by default.

### Mouse button inputs
For mouse button inputs, you must precise the button that must be pressed/released/hold by adding a `mouse button` field in the description.
See [this SFML enumeration](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Mouse.php#a4fb128be433f9aafe66bc0c605daaa90) for the available buttons.
You can also use `Any` to trigger the input when any button is pressed.
If the `mouse button` field is missing, `Any` is used by default.

### Joystick inputs
For joystick inputs, you must precise the ID of the joystick by adding a `joystick id` in the description.
The IDs are simply integers and are 0-indexed.
If the field is absent, the input is triggered for any joystick.

For axis inputs, you must also precise which axis must be moved by adding a `axis` field.
See [this SFML enumeration](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Joystick.php#a48db337092c2e263774f94de6d50baa7) for the available axes.

{% include warning.html content="
Any is not supported for axes.
You must always specify an axis.
" %}

For button inputs, you must precise which button must be pressed/released/hold by adding a `joystick button` field.
A button is identified by an integer.
You can also use `Any` to trigger the input when any button is pressed.
If the `joystick button` field is missing, `Any` is used by default.

# Input handler
The [input handler]({{ "/documentation/alpha/c++/classengine_1_1input_1_1InputHandler.html" | relative_url }}) is simply the class that registers and, well, handles the inputs.
You can manually connect inputs by giving the event type and the description.
You can also load a configuration from a JSON object and save the current configuration to a JSON object.

The first way allows to disconnect inputs later by using the [input connection](http://localhost:4000/documentation/alpha/c++/classengine_1_1input_1_1InputHandler_1_1InputConnection.html) returned.

# Configuration file
The `input` field of the `media` section of the [game description](FileManager#game-description) gives the path to the configuration file of the input handler.
The file is simply a collection of descriptions as shown here:

```json
{
  "eventType" : {
    "type": "Closed",
    "control": true,
    "lshift": true,
    "ralt": true,
    "state": "mainmenu"
  },
  "ChangeState": {
    "type": "KeyReleased",
    "key": "Space"
  }
}
```