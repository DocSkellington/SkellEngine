--[[!
    \defgroup Lua Lua functions

    The Lua API.
--]]

--[[! \addtogroup Lua
    @{
--]]

entityManager = inheritsFrom(nil, "entityManager")

--[[!
    @brief Creates a new empty entity with the given type.

    The entity is created without any components. The new entity is stored in the Entity Manager but not in the System Manager

    @param type The type of the entity to add
--]]
function entityManager:addEntity(type) end

--[[!
    @brief Creates a new entity with the given type and components.

    The entity is created with the given components. The new entity is stored in the Entity Manager and then passed to the System Manager to be used by the systems.

    @param type The string telling the type of the entity to add
    @param components The table describing the components
--]]
function entityManager:addEntity(type, components) end

--! @}