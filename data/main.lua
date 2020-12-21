require "menu"

button = image.load("assets/system/ui/button.png")

function main_loop()
    if controls.mouse.down then 
        print("X", controls.mouse.x)
        print("Y", controls.mouse.y)
    end

    width = game.window.width
    height = game.window.height

    render.setDrawColor(0, 255, 255, 255)
    render.clear()

    render.showSurface(button)
    render.setDrawColor(255, 0, 0, 255)
    render.drawLine(0, 0, width, height)

    render.setDrawColor(0, 255, 0, 255)
    render.drawFillRect(0, 0, width/2, height/4)
end

function shutdown()
    engine.freeSurface(button)
end
