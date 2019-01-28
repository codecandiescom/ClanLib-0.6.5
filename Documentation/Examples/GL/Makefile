EXAMPLE_BIN=    gl1 clanfog

include ../Makefile.conf

gl1: Intermediate/gl1.o
	$(CXX) -g3 -ggdb3 Intermediate/gl1.o -o gl1 -lclanGL -lclanApp -lclanDisplay -ldl

clanfog: Intermediate/clanfog.o
	$(CXX) -g Intermediate/clanfog.o -o clanfog -lclanGL -lclanApp -lclanDisplay -ldl
