require "menu"

button = game.IMGLoad("assets/system/ui/button.png")

function main_loop()
    if game.getScene() == 0 then
        menu_render()
        game.showSurface(button)
    end
end

function shutdown()
    game.freeSurface(button)
end
