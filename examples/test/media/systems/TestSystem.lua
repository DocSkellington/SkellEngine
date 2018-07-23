function init()
    entity = entMan:addEntity("test")
    entity:addComponent("position", {x=2, y=38})
    print(entity:hasComponent("position"))
    position = entity:getComponent("position")
    print(position:get("x"))
    print(position:get("y"))
end

function update(deltatime, view)
end
