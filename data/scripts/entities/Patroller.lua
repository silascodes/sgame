
PatrollerState = {
    Observing = 0,
    Walking = 1
}

Patroller = {
    state = PatrollerState.Walking,
    observeTime = 0,
    pathNode = 1,
    pathNodes = {{15 * 32, 20 * 32}, {30 * 32, 25 * 32}, {25 * 32, 10 * 32}}
}

function Patroller:New()
    Log_Info("new patroller")

    o = {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function Patroller:Spawn(ent)
    Log_Info("spawning patroller")

    self.entity = ent
    self.entity:SetSprite("data/sprites/test-gray.png")
    self.entity:OnEvent("ReachedDestination", "OnReachedDestination")
    self.entity:SetMoveDestination(15 * 32, 20 * 32)
    --self.entity:SetMoveDestination(self.pathNodes[self.pathNode][0], self.pathNodes[self.pathNode][1])
    self.entity:SetThinkTime(250)
    --self.entity:SetMoveDestination(25 * 32, 20 * 32)
end

function Patroller:Think(elapsedMs)
    -- check for enemies and stuff

    if self.state == PatrollerState.Observing then
        self.entity:SetThinkTime(100)
        self.observeTime = self.observeTime - elapsedMs
        Log_Info(self.observeTime)
        if self.observeTime <= 0 then
            Log_Info("Patroller finished observing")
            self.pathNode = self.pathNode + 1
            if self.pathNode > 3 then
                Log_Info("Patroller path nodes wrapping")
                self.pathNode = 1
            end
            self.state = PatrollerState.Walking
            self.entity:SetMoveDestination(self.pathNodes[self.pathNode][1], self.pathNodes[self.pathNode][2])
        end
    elseif self.state == PatrollerState.Walking then
        self.entity:SetThinkTime(250)
    end
end

function Patroller:OnReachedDestination()
    Log_Info("Patroller reached destination")

    self.state = PatrollerState.Observing
    self.entity:SetThinkTime(100)
    self.observeTime = 500
end
