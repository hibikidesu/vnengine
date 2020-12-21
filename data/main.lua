require "menu"

button = image.load("assets/system/ui/button.png")

function main_loop()
	if controls.mouse.down then
		print("X", controls.mouse.x)
		print("Y", controls.mouse.y)
	end
	local w = window.width
	print(w)
end

function shutdown() 
	image.free(button) 
end
