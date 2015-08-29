if game==nil then game = {} end

local mxSpeed = 100;

local up
function up(entity, pressed)
  addSpeed(entity, 0, mxSpeed * (pressed and -1 or 1));
end

local down
function down(entity, pressed)
  addSpeed(entity, 0, mxSpeed * (pressed and 1 or -1));
end

local right
function right(entity, pressed)
  addSpeed(entity, mxSpeed * (pressed and 1 or -1), 0);
end

local left
function left(entity, pressed)
  addSpeed(entity, mxSpeed * (pressed and -1 or 1), 0);
end

local input
function input(entity, key, pressed)
  print(entity, key, pressed)
  if key == "Up" then up(entity, pressed) end
  if key == "Down" then down(entity, pressed) end
  if key == "Right" then right(entity, pressed) end
  if key == "Left" then left(entity, pressed) end
end

game["2"] = input
