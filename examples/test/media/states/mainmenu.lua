function onCreate()
    registerCallback("ChangeState", onChangeState, "mainmenu")
    game.systemManager:loadSystems("graphicalorthogonal")
    game.level:changeLevel("tutorial")
end

function onChangeState(event)
    game.stateManager:switchTo("game")
    game.stateManager:remove("mainmenu")
end

function update(deltaTime)
    game.level:update(deltaTime)
    game.systemManager:update(deltaTime)
end

function draw(target)
    game.level:draw(target)
    game.gui:draw()
end