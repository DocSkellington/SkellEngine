function onCreate()
    registerCallback("ChangeState", onChangeState, "mainmenu")
end

function onChangeState(event)
    game.stateManager:switchTo("game")
    game.stateManager:remove("mainmenu")
end