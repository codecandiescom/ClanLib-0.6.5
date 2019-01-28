EXAMPLE_BIN=vorbistest

include ../Makefile.conf

vorbistest: Intermediate/vorbistest.o
	@echo NOTE: This example is known to segfault at exit on some machines.
	$(CXX) Intermediate/vorbistest.o -o vorbistest -lclanVorbis -lclanApp -lclanSound

static: Intermediate/vorbistest.o
	$(CXX) -static Intermediate/vorbistest.o -o vorbistest -lclanVorbis -lclanApp -lclanSound -lclanCore -lvorbis -logg $(LINK_STATIC)
