-- Game is a table to store the script with id in game[id] = function.
-- Vars is where vars live. Each script has its vars in vars[id].
game = {}
vars = {}

function init(id)
  if vars[id] == nil then
    vars[id] = {}
  end
end
