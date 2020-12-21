button = {};
button.tex = image.load("assets/system/ui/button.png");
button.w, button.h = render.queryTexture(button.tex);

function menu_render()
    render.showTexture(button.tex, 50, 0, button.w, button.h);
end

function menu_shutdown() 
    image.free(button.tex)
end
