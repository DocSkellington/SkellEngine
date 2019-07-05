function init()
    entity = game.entityManager:addEntity("test")
    entity:addComponent("position", {x=2, y=38})
    print(entity:hasComponent("position"))
    position = entity:getComponent("position")
    position:set("x", 5.1)
    print(position:get("x"))
    print(position:get("y"))

    component = entity:addComponent("test", {t=2, e=1, s=1})

    print("Size of entity :" .. #entity)
    print(component:get("t"))
    component:set("t", 5)
    print(component:get("t"))

    for n, c in pairs(entity) do
        print(n, c)
        print(c:get("x"))
    end
end

function update(deltatime, view)
end

function checkComponents(entity)
    return true;
end
