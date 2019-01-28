EXAMPLE_BIN= font

include ../Makefile.conf


font: Intermediate/font.o
	$(CXX) Intermediate/font.o -o font -lclanCore -lclanApp -lclanDisplay

#note: fontcanvas doesn't work properly yet
fontcanvas: Intermediate/fontcanvas.o
	@echo The fontcanvas example is KNOWN NOT TO WORK
	$(CXX) Intermediate/fontcanvas.o -o fontcanvas -lclanCore -lclanApp -lclanDisplay

static: Intermediate/font.o Intermediate/fontcanvas.o
	$(CXX) -static  Intermediate/font.o -o font -lclanApp -lclanDisplay -lclanCore $(LINK_STATIC)
	$(CXX) -static  Intermediate/fontcanvas.o -o fontcanvas -lclanApp -lclanDisplay -lclanCore $(LINK_STATIC)
