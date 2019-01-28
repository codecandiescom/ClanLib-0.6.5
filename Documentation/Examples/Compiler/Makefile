
EXAMPLE_BIN=clan_compiler

include ../Makefile.conf

clan_compiler: Intermediate/clan_compiler.o
	$(CXX) ${CXXFLAGS} Intermediate/clan_compiler.o -o clan_compiler -lclanCore -lclanApp -lclanDisplay -lclanSound

static: Intermediate/clan_compiler.o
	$(CXX) -static ${CXXFLAGS} Intermediate/clan_compiler.o -o clan_compiler -lclanApp -lclanDisplay -lclanSound -lclanCore $(LINK_STATIC)
