EXAMPLE_BIN=res_test

include ../Makefile.conf

res_test: Intermediate/resources.o
	$(CXX) Intermediate/resources.o -o res_test -lclanCore -lclanApp -lclanDisplay

static: Intermediate/resources.o
	$(CXX) -static Intermediate/resources.o -o res_test -lclanApp -lclanDisplay -lclanCore $(LINK_STATIC)
