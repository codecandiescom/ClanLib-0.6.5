EXAMPLE_BIN= screenshot
include ../Makefile.conf

libs = -lclanApp -lclanCore -lclanDisplay -lclanPNG
cppflags = 

OBJF =	Intermediate/writepng.o \
		Intermediate/cl_writepng.o \
		Intermediate/screenshot.o

screenshot:  $(OBJF)
	@echo "Building executable 'screenshot'..."
	$(CXX) $(libs) -o screenshot $(OBJF)
