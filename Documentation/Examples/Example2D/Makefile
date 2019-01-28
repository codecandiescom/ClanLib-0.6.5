
EXAMPLE_BIN=    example2d

include ../Makefile.conf

LIBS=../../../Libs

static: Intermediate/example2d.o
	$(CXX) ${CXXFLAGS} -static -g Intermediate/example2d.o -o example2d -lclanApp -lclanDisplay -lHermes -L/usr/X11R6/lib -lX11 -lXext -lXxf86vm -lclanCore -lz -lpthread 

example2d: Intermediate/example2d.o
	$(CXX) ${CXXFLAGS} -g Intermediate/example2d.o -o example2d -lclanApp -lclanDisplay -lpthread -lclanCore
