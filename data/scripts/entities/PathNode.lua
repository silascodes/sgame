
PathNode = {

}

function PathNode:New()
    o = {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function PathNode:Spawn(ent)
    self.entity = ent
    self.entity:SetSprite("data/sprites/test-green.png")
end

function PathNode:Think(elapsedMs)

end
