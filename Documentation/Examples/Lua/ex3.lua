-- Example 3

CL_Display:set_videomode(320, 200, 16,0)

my_canvas = CL_Canvas:new(320, 200)
my_cliprect = CL_ClipRect:new(20, 20, 280, 160)
my_canvas:push_clip_rect(my_cliprect)

my_canvas:fill_rect(0, 0, 320, 200, 0.0, 0.0, 0.5, 1.0)
my_canvas:draw_line(10, 10, 300, 100, 1.0, 1.0, 1.0, 1.0)

my_canvas:pop_clip_rect()
my_surface = CL_Surface:create(my_canvas, FALSE)

my_surface:put_screen(0, 0)
CL_Display:flip_display()
CL_System:sleep(5000)

my_canvas:delete()
my_surface:delete()
my_cliprect:delete()

