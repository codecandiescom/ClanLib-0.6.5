EXAMPLE_BIN=initdisplay

include ../Makefile.conf

initdisplay: Intermediate/init_display.o
	$(CXX) Intermediate/init_display.o -o init_display -lclanCore  -lclanApp -lclanDisplay

static: Intermediate/init_display.o
	$(CXX) -static Intermediate/init_display.o -o init_display -lclanApp -lclanDisplay -lclanCore $(LINK_STATIC)
