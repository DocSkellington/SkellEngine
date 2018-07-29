function init()
    entity = game.entityManager:addEntity("test")
    entity:addComponent("position", {x=2, y=38})
    print(entity:hasComponent("position"))
    position = entity:getComponent("position")
    position:set("x", 5.1)
    print(position:get("x"))
    print(position:get("y"))

    f = Vector2f.new()
    f.x = 5
    f.y = 90
    print(f.x, f.y)

    f2 = Vector2f.new(2, 3)
    print(f2.x, f2.y)

    f3 = f + f2
    print(f3.x, f3.y)

    print(f3 == f2)
    print(f3 == f+f2)
    print(f3 ~= f)

    r1 = DoubleRect.new(0, 0, 2, 2)
    r2 = DoubleRect.new(1, 1, 3, 3)
    print(r1.left, r1.top, r1.width, r1.height)

    intersection = DoubleRect.new()
    r1:intersects(r2, intersection)
    print(intersection.left, intersection.top, intersection.width, intersection.height)
    print(intersection == DoubleRect.new(1, 1, 1, 1))

    entity2 = game.entityManager:addEntity("TestGet", {position = {1.2, 3.4}})
    pos2 = entity2:getComponent("position")
    print(pos2:get("x"), pos2:get("y"))

    entity3 = game.entityManager:addEntity("ohh")
    pos3 = entity3:getComponent("position")
    print(pos3)

    entity3 = game.entityManager:getEntity("TestGet", {"position"})
    print(entity2)
    print(entity3)
    pos3 = entity3:getComponent("position")
    print(pos2:get("x") == pos3:get("x"), pos2:get("y") == pos3:get("y"))

    game.entityManager:removeEntity(entity3)
    print(entity3)
    print(entity3 == nil)

    entity4 = game.entityManager:getEntity("TestGet", {"position"})
    print(entity4)
end

function update(deltatime, view)
    view:rotate(4 * deltatime / 10000)
end

function checkComponents(entity)
    return true;
end
