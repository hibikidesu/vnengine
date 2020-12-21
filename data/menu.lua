-- List of buttons
buttons = {
    {
        tex = image.load("assets/system/ui/button.png"),
        x = 0,
        y = 0
    }
}

-- Query texture for all buttons
for i=1, #buttons do
    local w, h = render.queryTexture(buttons[i].tex);
    buttons[i].w = w;
    buttons[i].h = h;
end

-- Check if button press
function check_on_button(x, y) 
    for i=1, #buttons do
        if (x >= buttons[i].x and x <= (buttons[i].x + buttons[i].w) and
            y >= buttons[i].y and y <= (buttons[i].y + buttons[i].h)) then
            engine.setScene(i);
            render.clear();
        end
    end
end

function menu_render()
    render.setDrawColor(255, 255, 255, 255);
    render.clear();
    render.showTexture(buttons[1].tex, buttons[1].x, buttons[1].y, buttons[1].w, buttons[1].h);

    -- Handle mousedown on button
    if controls.mouse.down then
        check_on_button(controls.mouse.x, controls.mouse.y);
    end
end

function menu_shutdown() 
    image.free(button.tex)
end
