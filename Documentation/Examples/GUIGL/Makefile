EXAMPLE_BIN = opengl

include ../Makefile.conf

OBJF = Intermediate/button_opengl.o \
	Intermediate/opengl.o \
	Intermediate/stylemanager_opengl.o \
	Intermediate/window_opengl.o



opengl: $(OBJF)
	$(CXX) -g $(OBJF) -o opengl -lclanGUI -ldl -lclanGL -lclanCore -lclanApp -lclanDisplay