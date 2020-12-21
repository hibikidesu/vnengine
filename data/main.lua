require "menu"

button = image.load("assets/system/ui/button.png")

function main_loop()
	if controls.mouse.down then
		print("X", controls.mouse.x)
		print("Y", controls.mouse.y)
	end
	local w = window.width
	local h = window.height

	render.setDrawColor(255, 255, 255, 255)
	render.clear()

	render.show
end

function shutdown() 
	image.free(button) 
end
