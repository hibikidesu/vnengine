button = {};
button.tex = image.load("assets/system/ui/button.png");
button.x = 50; 
button.y = 50;
button.w, button.h = render.queryTexture(button.tex);

function check_on_button(x, y) 
    if (x >= button.x and x <= (button.x + button.w) and
        y >= button.y and y <= (button.y + button.h)) then
        print("YES")
    end
end

function menu_render()
    render.setDrawColor(255, 255, 255, 255);
    render.clear();
    render.showTexture(button.tex, button.x, button.y, button.w, button.h);

    -- Handle mousedown on button
    if controls.mouse.down then
        check_on_button(controls.mouse.x, controls.mouse.y);
    end
end

function menu_shutdown() 
    image.free(button.tex)
end
