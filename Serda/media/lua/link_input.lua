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
    if pressed > 10 then
      v.right = pressed/100
      v.left = 0
    elseif pressed < -10 then
      v.right = 0
      v.left = -pressed/100
    else
      v.right = 0
      v.left = 0
    end
  end
  if key == "JoyY" then
    if pressed > 10 then
      v.down = pressed/100
      v.up = 0
    elseif pressed < -10 then
      v.down = 0
      v.up = -pressed/100
    else
      v.down = 0
      v.up = 0
    end
  end
  setSpeed(entity, (v.right - v.left) * mxSpeed, (v.down - v.up) * mxSpeed);
end
game[id] = input
