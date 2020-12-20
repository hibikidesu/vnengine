require "menu"
require "utils.button"
 
function main_loop()
    if game.getScene() == 0 then
        menu_render()
    end
end
