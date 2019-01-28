EXAMPLE_BIN= mouse mousesignal

include ../Makefile.conf

warning:
        @echo NOTE: This example is known to have a graphic distortion when moving the mouse.

mouse: warning Intermediate/mouse.o
	$(CXX) Intermediate/mouse.o -o mouse -lclanCore -lclanApp -lclanDisplay

mousesignal: warning Intermediate/mousesignal.o
	$(CXX) -g3 Intermediate/mousesignal.o -o mousesignal -lclanCore -lclanApp -lclanDisplay

static: Intermediate/mouse.o Intermediate/mousesignal.o
	$(CXX) -static Intermediate/mouse.o -o mouse -lclanApp -lclanDisplay -lclanCore $(LINK_STATIC)
	$(CXX) -static Intermediate/mousesignal.o -o mousesignal -lclanApp -lclanDisplay -lclanCore $(LINK_STATIC)
