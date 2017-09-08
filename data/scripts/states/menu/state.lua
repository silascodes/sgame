
root = ui:GetRootWidget()

mainScreen = Container.new()
newGameScreen = Container.new()
loadGameScreen = Container.new()

function MainScreenBtnClick()
    root:RemoveAllChildren()
    root:AddChild(mainScreen)
end

function NewGameBtnClick()
    root:RemoveChild(mainScreen)
    root:AddChild(newGameScreen)
end

function LoadGameBtnClick()
    -- load game (save name) parameter

    root:RemoveChild(mainScreen)
    root:AddChild(loadGameScreen)
end

function QuitBtnClick()
    client:Quit()
end

function StartGameBtnClick()
    -- new game parameter

    game:PushState("game")
end

function Init_MainScreen()
    local newGameBtn = Button.new()
    newGameBtn:SetPosition(100, 350)
    newGameBtn:SetSize(150, 30)
    newGameBtn:SetText("New Game")
    newGameBtn:SetOnMouseClick("NewGameBtnClick")
    mainScreen:AddChild(newGameBtn)

    local loadGameBtn = Button.new()
    loadGameBtn:SetPosition(100, 400)
    loadGameBtn:SetSize(150, 30)
    loadGameBtn:SetText("Load Game")
    loadGameBtn:SetOnMouseClick("LoadGameBtnClick")
    mainScreen:AddChild(loadGameBtn)

    local quitBtn = Button.new()
    quitBtn:SetPosition(100, 450)
    quitBtn:SetSize(150, 30)
    quitBtn:SetText("Quit")
    quitBtn:SetOnMouseClick("QuitBtnClick")
    mainScreen:AddChild(quitBtn)
end

function Init_NewGameScreen()
    local backBtn = Button.new()
    backBtn:SetPosition(10, 680)
    backBtn:SetSize(150, 30)
    backBtn:SetText("Back")
    backBtn:SetOnMouseClick("MainScreenBtnClick")
    newGameScreen:AddChild(backBtn)

    local startBtn = Button.new()
    startBtn:SetPosition(1120, 680)
    startBtn:SetSize(150, 30)
    startBtn:SetText("Start Game")
    startBtn:SetOnMouseClick("StartGameBtnClick")
    newGameScreen:AddChild(startBtn)

    local levelListBox = ListBox.new()
    levelListBox:SetPosition(10, 10)
    levelListBox:SetSize(1260, 660)
    newGameScreen:AddChild(levelListBox)

    levelListBox:AddItem("Dark Rivers of the Heart")
    levelListBox:AddItem("Watchers")
    levelListBox:AddItem("Killjoy")
end

function Init_LoadGameScreen()
    local backBtn = Button.new()
    backBtn:SetPosition(10, 680)
    backBtn:SetSize(150, 30)
    backBtn:SetText("Back")
    backBtn:SetOnMouseClick("MainScreenBtnClick")
    loadGameScreen:AddChild(backBtn)

    local loadBtn = Button.new()
    loadBtn:SetPosition(1120, 680)
    loadBtn:SetSize(150, 30)
    loadBtn:SetText("Load Game")
    loadGameScreen:AddChild(loadBtn)
end

function Init()
    Log_Info("Menu Init")

    -- get list of saved games

    Init_MainScreen()
    Init_NewGameScreen()
    Init_LoadGameScreen()

    root:AddChild(mainScreen)
end

function RunFrame()

end
