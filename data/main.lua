require "menu"

print("game", game)
print("game.width", game.width)
print("game.window", game.window)
print("game.window.width", game.window.width)

function main_loop()
    if mouse.down == true then
        print("down")
        print("x: ", mouse.x)
        print("y: ", mouse.y)
    end
end
