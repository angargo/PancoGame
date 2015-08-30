if game==nil then game = {} end

local mxSpeed = 100;
local length = 100;
local down = {};
local up = {};

local f
function f(entity)
  x, y = 0, 0 --getPosition(entity);
  if up[entity] == nil then up[entity] = y end
  if down[entity] == nil then down[entity] = y + length end

  if y <= up[entity] then setSpeed(entity, 0, mxSpeed) end
  if y >= down[entity] then setSpeed(entity, 0, -mxSpeed) end
end

game["3"] = f
