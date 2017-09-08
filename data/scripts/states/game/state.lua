
function MenuBtnClick()
    Log_Info("MenuBtnClick")

    game:PopState()
end

function Init()
    Log_Info("Game Init")

    state:ChangeMap("test1")

    local root = ui:GetRootWidget()

    local menuBtn = Button.new()
    menuBtn:SetPosition(0, 0)
    menuBtn:SetSize(150, 30)
    menuBtn:SetText("Menu")
    menuBtn:SetOnMouseClick("MenuBtnClick")
    root:AddChild(menuBtn)

    -- TODO: load map & spawn entities?
end

function RunFrame()

end
