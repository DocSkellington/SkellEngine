How states work       {#states}
===
[TOC]

In our implementation, a state represents a part of a game. Typically, these parts are:
  - The `introduction` shows the developpers' and editor's logo (for example);
  - The `main menu` that allows to start a new game, load a save file and so on;
  - The `game` that is the actual game;
  - The `pause` that pauses the game;
  - The `game over` that displays a good or bad ending to the game.

We propose already defined states. Each state has a differente list of overridable settings (see each state's documentation for more information).

The rest of this page explains briefly how the user can use states.

# How a state works {#states_work}
States are managed by the State Manager. The manager has a queue with all the states currently in the game. The front of the queue is the active state.

## Switching to a new state {#states_switch}
When the user wants to switch to a new state, the following actions are performed:
  - The `deactivate` function of the current active state is called;
  - If the state is already in the game, the `activate` function is called and the state becomes the active state;
  - If the state is not yet in the game, the state is created, the functions `onCreate` and `activate` are called (in this order);

## Removing a state {#states_remove}
The user can also remove a state from the queue. When the state is destroyed, the `onDestroy` function is called to release the data.

## Update and draw {#states_update}
Each frame, the active state is updated (thanks to the `update` function). If it is transcendant, the next state in the queue is updated too (and we keep going until the last state or if we fall on a non-transcendant state). The active state is then drawn (thanks to the `draw` function). If it is transparent, the next state in the queue is drawn too (and we keep going until the first non-transparent state or the last state of the queue).

# Example {#states_example}
\todo When the states are defined, add examples on how to use them