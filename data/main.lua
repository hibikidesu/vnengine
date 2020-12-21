require "menu"

-- button = game.IMGLoad("assets/system/ui/button.png")

print("Tests:")
print("---\ngame", game)
print("game.window", game.window)
print("game.window.width", game.window.width)
print("game.window.height", game.window.height)
print("---\ncontrols", controls)
print("controls.mouse", controls.mouse)
print("controls.mouse.down", controls.mouse.down)
print("controls.mouse.x", controls.mouse.x)
print("controls.mouse.y", controls.mouse.y)
print("---\nengine", engine)
print("engine.freeSurface", engine.freeSurface)
print("engine.getScene", engine.getScene)
print("engine.setScene", engine.setScene)
print("---\nimage", image)
print("image.load", image.load)
print("---\nrender", render)
print("render.showSurface", render.showSurface)
print("render.drawLine", render.drawLine)
print("render.drawFillRect", render.drawFillRect)
print("render.drawRect", render.drawRect)
print("render.setDrawColor", render.setDrawColor)
print(string.format("render.clear %s", render.clear)) -- y does this err if not formatted

function main_loop()
    if controls.mouse.down then 
        print("X", controls.mouse.x)
        print("Y", controls.mouse.y)
    end
end

-- function shutdown()
    -- engine.freeSurface(button)
-- end
