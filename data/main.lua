require "menu"

button = image.load("assets/system/ui/button.png")

function main_loop()
	if controls.mouse.down then
		print("X", controls.mouse.x)
		print("Y", controls.mouse.y)
	end
	render.showTexture(button, 0, 0)
end

function shutdown() 
	image.free(button) 
end
