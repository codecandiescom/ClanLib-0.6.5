-- Example 2
-- Lenny Palozzi

CL_Display:set_videomode(640, 480, 16,0)

mx = CL_Mouse:get_x()
my = CL_Mouse:get_y()
old_mx = mx
old_my = my

while CL_Keyboard:get_keycode(CL_KEY_ESCAPE) == FALSE
do
	mx = CL_Mouse:get_x()
	my = CL_Mouse:get_y()

	if mx ~= old_mx or my ~= old_my then
		CL_Display:fill_rect(mx, my, mx+8, my+8, call(random, {}), call(random, {}), call(random, {}), 1.0)
		CL_Display:flip_display()
		CL_Display:sync_buffers()
		old_mx = mx;
		old_my = my;
	end
	CL_System:sleep(10)
	CL_System:keep_alive()
end




