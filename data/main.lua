require "menu"

button = {};
button.tex = image.load("assets/system/ui/button.png");
button.w, button.h = render.queryTexture(button.tex);

function main_loop()
	if controls.mouse.down then
		print("X", controls.mouse.x)
		print("Y", controls.mouse.y)
	end
	render.showTexture(button.tex, 50, 0, button.w, button.h);
end

function shutdown() 
	image.free(button.tex) 
end
