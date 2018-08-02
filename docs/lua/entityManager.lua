--- Entity Manager.
-- The entity manager handles the entity of the engine. One can add, remove and get entities.
-- @classmod entityManager

--- Creates a new entity.
-- The new entity does not have any components and is not added to the systemManager. The user must manually do it (after the components have been defined) for the entity to be used.
-- @param type The type of the entity to construct and add
-- @return The new entity
function entityManager:addEntity(type)
end

--- Creates a new entity with the given components.
-- The new entity is added to the systemManager. So, don't forget any components in the list.
-- @param type The type of the entity to construct and add
-- @param components The table describing the components and their fields
-- @return The new entity
function entityManager:addEntity(type, components)
end

--- Gets an entity.
-- The first matching entity is returned
-- @param type The type of the entity to get
-- @return The first matching entity
function entityManager:getEntity(type)
end

--- Gets an entity that has at least the given components.
-- The first matching entity is returned.
-- The entity can have more components than the listed ones
-- @param type The type of the entity to get
-- @param components The list of the names of the comonents the entity must have
-- @return The first matching entity
function entityManager:getEntity(type, components)
end

--- Removes an entity from the game.
-- It also removes the entity from the systems. Therefore, the entity is completely removed from the engine.
-- @param entity The entity to remove
-- @return Whether the entity could be removed or not
function entityManager:removeEntity(entity)
end