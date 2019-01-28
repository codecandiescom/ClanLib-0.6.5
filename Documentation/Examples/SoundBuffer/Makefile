EXAMPLE_BIN= soundbuffer

include ../Makefile.conf

soundbuffer: Intermediate/soundbuffer.o
	@echo NOTE: This example is known to segfault at exit on some machines.
	@g++ -g3 Intermediate/soundbuffer.o -o soundbuffer -lclanCore -lclanApp -lclanSound
