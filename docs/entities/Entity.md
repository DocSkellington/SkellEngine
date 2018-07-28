How entities work       {#entities}
======
[TOC]

In our ECS architecture, an entity is an object that contains components. Each entity has a _type_, for example, `player`; multiple entities can share the same type.

# Creation {#entities_creation}
The Entity Manager allows for creating new entities. The new entity can be initiliased:
  - As empty. In this case, the user must add the components and, then, add the entity to the System Manager.
  - With a JSON table describing the components to add to the entity. In this case, the entity is immediately added to the System Manager.

# Get and remove an entity {#entities_get_remove}
The user can retrieve an entity from the Entity Manager.