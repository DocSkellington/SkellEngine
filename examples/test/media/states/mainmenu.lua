function onCreate()
    setIsTransparent(true)
    registerCallback("ChangeState", onChangeState, "mainmenu")
end

function onChangeState(event)
    print("mainmenu:onChangeState")
    game.stateManager:switchTo("game")
    game.stateManager:remove("mainmenu")
    print("done")
end

function activate()
    print("MAINMENU")
end