-- Lua script example
-- Lenny Palozzi

CL_Display:set_videomode(640, 480, 16,0)
CL_Display:clear_display()

msg = call(format, {"Video Mode: %dx%d", CL_Display:get_width(), CL_Display:get_height()} )
print(msg)

my_rect = CL_ClipRect:new(100, 100, 300, 300)

-- not working... CL_Display:push_clip_rect(my_rect)

my_canvas = CL_Canvas:new(320, 200)
-- canvas has changed, fill, draw.. gone? use CL_Target?
--my_canvas:fill_rect(0, 0, 320, 200, 0.0, 0.0, 0.5, 1.0)
--my_canvas:draw_line(10, 10, 300, 100, 1.0, 1.0, 1.0, 1.0)

--my_surface = CL_Surface:create(my_canvas)
-- C++ code: CL_Surface *surf_canvas = CL_Surface::create(&canvas);

while CL_Keyboard:get_keycode(CL_KEY_ESCAPE) == FALSE
do
	a = call(random, {800})
	b = call(random, {600})
	
	CL_Display:draw_line(a, b, a+20, b+20, call(random, {}), call(random, {}), call(random, {}), 1.0)
	CL_Display:flip_display(TRUE)
	CL_Display:sync_buffers()
	
	CL_System:keep_alive()
	CL_System:sleep(10)
end

--CL_Display:pop_clip_rect()
my_rect:delete()
my_canvas:delete()
--my_surface:delete()












