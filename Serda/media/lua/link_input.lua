local id = tostring(2);
init(id);
local s = vars[id]; -- Script specific variables in this table.

local mxSpeed = 100;

local input
function input(entity, key, pressed)
  if s[entity] == nil then
    s[entity] = {}
    local v = s[entity];
    v.up, v.down, v.right, v.left = 0, 0, 0, 0
  end

  local v = s[entity];
  if key == "Up" then v.up = pressed and 1 or 0 end
  if key == "Down" then v.down = pressed and 1 or 0 end
  if key == "Right" then v.right = pressed and 1 or 0 end
  if key == "Left" then v.left = pressed and 1 or 0 end
  if key == "JoyX" then
    v.right = pressed/100
    v.left = 0
  end
  if key == "JoyY" then
    v.up = pressed/100
    v.down = 0
  end
  setSpeed(entity, (v.right - v.left) * mxSpeed, (v.down - v.up) * mxSpeed);
end
game[id] = input
