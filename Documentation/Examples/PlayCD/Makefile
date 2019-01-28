EXAMPLE_BIN= cdplay

include ../Makefile.conf

cdplay: Intermediate/cdplay.o
	$(CXX) -g ${CXXFLAGS} Intermediate/cdplay.o -o cdplay -lclanCore -lclanApp  -lclanSound
