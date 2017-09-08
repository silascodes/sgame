
PlayerSpawn = {

}

function PlayerSpawn:New()
    o = {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function PlayerSpawn:Spawn(ent)
    self.entity = ent
    self.entity:SetSprite("data/sprites/test-green.png")
end

function PlayerSpawn:Think()

end
