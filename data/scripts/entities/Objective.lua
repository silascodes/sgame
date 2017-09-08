
Objective = {

}

function Objective:New()
    o = {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function Objective:Spawn(ent)
    self.entity = ent
end

function Objective:Think(elapsedMs)

end
