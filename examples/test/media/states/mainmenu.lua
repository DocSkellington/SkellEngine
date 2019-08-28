function onCreate()
    registerCallback("WindowClosed", game.closeEngine, "mainmenu")

    registerCallback("ChangeState", onChangeState, "mainmenu")
    game.systemManager:loadSystems("graphicalorthogonal")
    game.level:changeLevel("tutorial")
    
    path = game.gameDescription.media.baseMediaPath / "Black.txt"
    theme = Theme.new(tostring(path))
    Theme.setDefault(theme)

    button = Button.create("BUTTON")
    button:setPosition(200, 100)
    print(button:getPosition().x .. " " .. button:getPosition().y)
    button:setSize(150, 300)
    button:setVisible(true)
    print(button:getFullSize().x .. " " .. button:getFullSize().y)

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