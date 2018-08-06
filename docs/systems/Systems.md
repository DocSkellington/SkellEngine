How systems work    {#systems}
===
[TOC]

In our ECS architecture, a system is an object that defines logical operations on entities and their components. Each system has a unique name. The systems are managed by the System Manager. In C++, the System Manager can be accessed with a reference to the Context. In Lua, it is not possible to access the System Manager.

# How a system works
## Creation
Systems are automatically created when the Game State is created. The list of needed systems must be present in the file describing the Game State (see [the Game State documentation](@ref states_game)).

The system can define as many variables as it needs during its creation.

Removing a system during the game execution is not recommended as it may lead to unexpected behaviour.

## Links with entities
Once the system is created, entities can be added to it. To do so, the system must define which kind of entities it accepts by explicitily listing every needed components.

It's also possible to remove entities on the fly.

## Update function
The update function is the core of a system as it defines what the system does each frame. The user can iterate over every entity the system can manage and read and modify the components of these entities.

# Graphical systems
The definition of a graphical system is only possible in C++ as it requires performance and the full SFML API. The name of the system must start with "graphical" (case does not matter).

# Define a new system
## C++
To define a new system, the user must create a new class that inherits from System and override the functions
~~~{.cpp}
public:
  bool update(sf::Int64, sf::View&) override;
protected:
  bool checkComponents(engine::entities::Entity::Ptr entity) const override;
~~~

For a graphical system, the following function must also be overriden:
~~~{.cpp}
void draw(sf::RenderWindo*, unsigned int layer, sf::View) override;
~~~

### Registering
To register the system T, the user must create a static object of type System::RegisterSystem<T> (declare it in header file and implement it in cpp file). For example, let's say we want to register the system PositionSystem under the name `position`. So, in header file, we write
~~~{.cpp}
#include "systems/System.h"

/* ... */

class PositionSystem : public System {
  /* ... */

private: // You don't have to make it private but it's useless for other classes
  static System::RegisterSystem<PositionSystem> rsps;
}
~~~

And, then, in the code file, we write
~~~{.cpp}
#include "PositionSystem.h"

/* ... */

System::RegisterSystem<PositionSystem> PositionSystem::rsps("position");
~~~

## Lua
The user only has to create a file with the ".lua" extension in the folder `systems` (see [the game description documentation](@ref json_game_description)). The name of the system for the engine depends on the name of the file. If the name of the file ends with "System" or "system", then the "S/system" is removed to keep only the name of the system.

The following functions can be defined and will be automatically called by the engine:
- `init()` when the system is created. By default, it does nothing.
- `update(deltaTime, view)` each frame. By default, it does nothing.
  - `deltaTime` is an integer with the time since the last update (in microseconds).
  - `view` is the sf::View currently in use.
- `checkComponents(entity)` to check if an entity can be added to the system. It must return a boolean. By default, it returns false.
  - `entity` is the entity.
- `destroy()` when the system is removed from the engine. By default, it does nothing.