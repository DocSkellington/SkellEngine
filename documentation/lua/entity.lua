--- An entity.
-- An entity contains (and is described by) components. An entity also has a type (it does not have to be unique).
--
-- The class supports the # operator to get the size.
-- It's also possible to iterate over the components of the entity with pairs(entityVariable). Beware that pairs actually returns a 3-tuple (index, name of the component, the component).
-- @classmod entity

--- Constructs and adds a new component.
--
-- The type of the component must be known and the entity can not already have this component. Otherwise, nil value is returned and the entity is not modified.
--
-- The values of the component get their default values. See the appropriate component documentation for more information.
-- @param componentType The type of the component.
-- @return The component if the type is known and not yet in the entity, nil otherwise
function entity:addComponent(componentType)
end

--- Constructs and adds a new component.
--
-- The type of the component must be known and the entity can not already have this component. Otherwise, nil value is returned and the entity is not modified.
--
-- The default values of the component can be overriden thanks to componentTable. See the appropriate component documentation for more information.
-- @param componentType The type of the component.
-- @param componentTable The table describing the values of the component.
-- @return The component if the type is known and not yet in the entity, nil otherwise
function entity:addComponent(componentType, componentTable)
end

--- Checks if the entity has the given component.
-- @param componentType The type of the component.
-- @return Whether the entity has a component of the appropriate type.
function entity:hasComponent(componentType)
end

--- Returns the component of the given type.
-- @param componentType The type of the component.
-- @return The component if the entity has a component of the appropriate type, nil otherwise
function entity:getComponent(componentType)
end

--- Returns a list of the component types this entity has
-- @return A table with the names of the components currently in this entity
function entity:getComponentsNames()
end

--- Returns the type of the entity
-- @return The type of the entity
function entity:getType()
end