EXAMPLE_BIN=plasma

include ../Makefile.conf

plasma: Intermediate/plasma.o Intermediate/plasma_asm.o
	$(CXX) Intermediate/plasma.o Intermediate/plasma_asm.o -o plasma -lclanCore -lclanApp -lclanDisplay
