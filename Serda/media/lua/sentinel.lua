local id = tostring(3);

init(id);
local s = vars[id]; -- Script specific variables in this table.

s.mxSpeed = 100;
s.length = 100;

local f
function f(entity)
  if s[entity] == nil then s[entity] = {} end
  local e = s[entity]; -- Entity-specific variables in this table.

  local x, y = getPosition(entity);

  if e.up == nil then e.up = y end
  if e.down == nil then e.down = y + s.length end

  if y <= e.up then setSpeed(entity, 0, s.mxSpeed) end
  if y >= e.down then setSpeed(entity, 0, -s.mxSpeed) end
end
game[id] = f
