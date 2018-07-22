function init()
    entity = entMan:addEntity("test")
    compoTest = entity:addComponent("compoTest")
    compoTest:set("IntTest", 5)
    print(compoTest:get("IntTest"))
    compoTest:set("String", "str is test")
    print(compoTest:get("String"))
    compoTest:set("Bool", false)
    print(compoTest:get("Bool"))
    print(compoTest:get("NotHere"))
    --table = {5, 7.5, 8, "String", true, {3.14, 9}}
    table = {bar="foo"}
    print(type(table))
    compoTest:set("Table", table)
    t,v = compoTest:get("Table")
    print(t,v)
    print("table: ")
    for k,i in pairs(t) do
        if (type(i) == "table") then
            for l, j in pairs(i) do
                print(k,l,j)
            end
        else
            print(k, i)
        end
    end

    pos = entity:addComponent("position")
    pos:set("x", 5)
    print(pos:get("x"), pos:get("y"))
end

function update(deltatime, view)
end
