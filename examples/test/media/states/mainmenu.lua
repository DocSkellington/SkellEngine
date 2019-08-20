function onCreate()
    registerCallback("ChangeState", onChangeState, "mainmenu")
    game.systemManager:loadSystems("graphicalorthogonal")
    game.level:changeLevel("tutorial")
    
    path = game.gameDescription.media.baseMediaPath / "Black.txt"
    theme = Theme.new(tostring(path))
    Theme.setDefault(theme)

    button = Button.create(String.new("BUTTON"))
    button:setPosition(200, 100)

    game.gui:add(button)
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