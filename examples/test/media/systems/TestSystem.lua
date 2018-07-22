function init()
    entity = entMan:addEntity("test")
    compoTest = entity:addComponent("compoTest")
    compoTest:set("IntTest", 5)
    print(compoTest:get("IntTest"))
end

function update(deltatime, view)
end
