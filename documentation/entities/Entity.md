How entities work       {#entities}
======
[TOC]

In our ECS architecture, an entity is an object that contains components. Each entity has a _type_, for example, `player`; multiple entities can share the same type. The entities are managed by the Entity Manager which has a list with every entity currently in the engine. In Lua, the Entity Manager can be accessed through `game.entityManager`, while in C++, it can be accessed with a reference to the Context.

# Creation {#entities_creation}
The Entity Manager allows for creating new entities with the function `addEntity`. The new entity can be initiliased:
  - As empty. In this case, the user must add the components and, then, add the entity to the System Manager.
  - With a JSON table describing the components to add to the entity. In this case, the entity is immediately added to the System Manager.

# Get and remove an entity {#entities_get_remove}
The user can retrieve an entity from the Entity Manager in two ways:
  - By providing only the `type` of the entity.
  - By providing the `type` and a list of components the entity must have (the entity can have other components).

In both cases, the function is called `getEntity` and the first found matching entity is returned.

Finally, the user can remove an entity from the Entity Manager with `removeEntity` and, therefore, from the game as the manager removes it from the systems, too. In Lua scripts, the variable used to store the entity will not (immediately?) be set to nil. So, I highly recommend you to do it yourself.

# Components {#entities_components}
Once the user has a reference to an entity, it's possible to add a new component (`addComponent`), check if the entity has a specific component (`hasComponent`) and to get a component (`getComponent`). It's impossible to remove a component.

It's also possible to iterate over the components.

# Example {#entities_examples}
Here are full examples with creating, getting and removing entities.

## C++ {#entities_examples_cpp}
In this example, we suppose we have the `context` variable as a reference to the Context of the engine.
~~~{.cpp}
// First, we create an empty entity
auto entity = context.entityManager->addEntity("newEntity");
// We then add a position component with default values
auto position = entity->addComponent("position");
position->set("x", 5.1);
position->set("y", 9);
// The three lines can be remplaced by:
entity->addComponent("position", {5.1, 9});
// or by:
entity->addComponent("position", {{"x", 5.1}, {"y", 9}});
// Don't forget to add it to the System Manager
context.systemManager->addEntity(entity);

// We can iterate over the components
for (auto &itr : entity) {
  if (itr.first == "position") {
    std::cout << "We have a position, Houston\n";
  }
}

// Now, we create an entity with a position component
context.entityManager->addEntity("entityPosition", {{"position", {48, 91.7}}});
// In this case, that's all you have to do. Of course, the function returns a reference to the created entity if you need to keep a direct access.

// Let's get the 'entityPosition'
auto entityPosition = context.entityManager->getEntity("entityPosition");
// You can filter more entities by doing
auto entityPosition = context.entityManager->getEntity("entityPosition", {"position", "other", "component"}) // it will only look after an 'entityPosition' with at least the three components (so, in this example, it will be an empty pointer as 'entityPosition' does not have 'other' and 'component' components)

// Eventually, we remove the 'entityPosition' entity from the engine
context.entityManager->removeEntity(entityPosition)

~~~

## Lua {#entities_examples_lua}
~~~{.lua}
-- First, we create an empty entity
entity = game.entityManager:addEntity("newEntity")
-- We then add a position component with default values
position = entity:addComponent("position")
position:set("x", 5.1)
position:set("y", 9)
-- The three lines can be remplaced by entity:addComponent("position", {x=5.1, y=9}) or by entity:addComponent("position", {5.1, 9})
-- Don't forget to add it to the System Manager
game.systemManager:addEntity(entity)

--- We can iterate over the components
for name, component in pairs(entity) do
  if name == "position"
    std::cout << "We have a position, Houston\n";
  end
end

-- Now, we create an entity with a position component
game.entityManager:addEntity("entityPosition", {position={48, 91.7}})
-- In this case, that's all you have to do. Of course, the function returns a reference to the created entity if you need to keep a direct access.

-- Let's get the 'entityPosition'
entityPosition = game.entityManager:getEntity("entityPosition")
-- You can filter more entities by doing
entityPosition = game.entityManager:getEntity("entityPosition", {"position", "other", "component"}) -- it will only look after an 'entityPosition' with at least the three components (so, in this example, it will be nil as 'entityPosition' does not have 'other' and 'component' components)

-- Eventually, we remove the 'entityPosition' entity from the engine
game.entityManager:removeEntity(entityPosition)
-- we set entityPosition to nil
entityPosition = nil
~~~