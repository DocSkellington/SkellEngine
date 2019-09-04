---
layout: tutorial
title: Event handler and events
shorttitle: Events
---

An [Event]({{ "/documentation/alpha/c++/classengine_1_1events_1_1Event.html" | relative_url }}) is the recommended way to transfer information between different parts of the engine (mainly, between systems).
Each event has a type and can contain data and entities.

{% include important.html content="
Events and inputs are two different parts of the engine.
Events are used to transfer information <em>within</em> the engine while inputs are used to transfer information <em>from</em> the player <em>to</em> the engine.
" %}

# Event handler
The [Event handler]({{ "/documentation/alpha/c++/classengine_1_1events_1_1EventHandler.html" | relative_url }}) is used to map an event type to callbacks.
A callback, here, is a function that has a `void(const engine::events::Event&)` signature.

To register a new callback for an event type, you can use [registerCallback]({{ "/documentation/alpha/c++/classengine_1_1events_1_1EventHandler.html#aa61d1c7799845725dfa23256ef81c0b7" | relative_url }}).
Callbacks can be active for every state or only for a specific state.
This means you can have a different callback per state for the same event type.
The function returns an [event connection]({{ "/documentation/alpha/c++/classengine_1_1events_1_1EventConnection.html" | relative_url }}) that can be used to check if the callback is still connected and to disconnect it.

{% include important.html content="
States and systems propose a specific registerCallback (only accessible inside a state/system).
Please use this function as it automatically remove callbacks when the state/system dies.
Doing this prevents trying to access an invalid callback.
" %}

Sending an event is as easy as calling one of the [sendEvent]({{ "/documentation/alpha/c++/classengine_1_1events_1_1EventHandler.html#a9d1a01bc1d18554c3cc88034f089fd79" | relative_url }}) overloads.
In other words, you can construct an event then send it or directly give the information the event must transfer to the event manager and let it construct the event.

{% include note.html content="
If no callback is registered for the event type, an info message is logged.
So, if nothing happens when you send an event, check the logger to see if it's because no callback was registered.
" %}

# Events
There are two ways to create events (as for [components](entities#Components)):
  1. Using an external event that stores automatically everything but it's impossible to check that the values and types are correct for the game logic.
  2. Only in C++: implementing a new class inheriting from [Event]({{ "/documentation/alpha/c++/classengine_1_1events_1_1Event.html" | relative_url }}) and overriding `create`.
  `create` receives a JSON object with the information to store in the event.
  This way, it's possible to check that the types and values are as expected.
  The class must be registered using [REGISTER_EVENT]({{ "/documentation/alpha/c++/Event_8h.html#aac46828485a9225091f23adcc85e9943" | relative_url }}) in order to be automatically used by the engine.
  If you want to be able to use the automatic getters and setters, you must register your variable members with [registerMember]({{ "/documentation/alpha/c++/classengine_1_1utilities_1_1MemberStorage.html#a242c8cf9d0875ad6b422ace3af18bef5" | relative_url }}).
  Otherwise, the event received by the callback must be casted to the exact type.

In both cases, pointers to entities can also be joined.