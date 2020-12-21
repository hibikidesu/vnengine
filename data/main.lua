require "menu"

function main_loop()
	local scene = engine.getScene();
	-- print(scene);
	if scene == 0 then
		menu_render();
	end
end

function shutdown() 
	menu_shutdown();
end
