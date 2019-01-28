EXAMPLE_BIN= minimum

include ../Makefile.conf

minimum: Intermediate/minimum.o
	$(CXX) Intermediate/minimum.o -o minimum -lclanCore -lclanApp

static: Intermediate/minimum.o
	$(CXX) Intermediate/minimum.o -o minimum -lclanApp -lclanCore
