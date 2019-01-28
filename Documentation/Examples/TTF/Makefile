EXAMPLE_BIN=ttf

include ../Makefile.conf


ttf: Intermediate/ttf.o

	$(CXX) Intermediate/ttf.o -o ttf -lclanCore -lclanTTF -lclanApp -lclanDisplay

static: Intermediate/ttf.o
	 $(CXX) -static Intermediate/ttf.o -o ttf  -lclanTTF -lclanApp -lclanDisplay -lclanCore -lfreetype $(LINK_STATIC)
