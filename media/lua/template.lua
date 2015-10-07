local id = tostring(--[[ Script ID --]]);
init(id);
local s = vars[id]; -- Script specific variables in this table.

local f
function f(entity) -- There may be other parameters.
  --[[ If several entities use this script:
  if s[entity] == nil then s[entity] = {} end
  local e = s[entity]; -- Entity-specific variables in this table.
  --]]

  -- Script definition.
end
game[id] = f
