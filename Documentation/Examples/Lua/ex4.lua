-- Example 4
-- Jeroen Janssen

CL_Display:set_videomode(640, 480, 16,0)

while CL_Keyboard:get_keycode(CL_KEY_ESCAPE) == FALSE
do
	CL_System:keep_alive()
	CL_System:sleep(10)
end




