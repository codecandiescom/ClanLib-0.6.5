EXAMPLE_BIN=canvas

include ../Makefile.conf

canvas: Intermediate/canvas.o
	$(CXX) Intermediate/canvas.o -o canvas -lclanCore -lclanApp -lclanDisplay

static:  Intermediate/canvas.o
	$(CXX) -static Intermediate/canvas.o -o canvas -lclanApp -lclanDisplay -lclanCore $(LINK_STATIC)
	
