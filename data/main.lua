require "menu"

print(game.changeScene(0))

function main_loop()
    if mouse.down == true then
        print("down")
        print("x: ", mouse.x)
        print("y: ", mouse.y)
    end
end
