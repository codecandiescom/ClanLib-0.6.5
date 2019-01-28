EXAMPLE_BIN=staticsoundprovider

include ../Makefile.conf

staticsoundprovider: Intermediate/static_soundprovider.o
	$(CXX) Intermediate/static_soundprovider.o -o static_soundprovider -lclanCore -lclanApp -lclanSound
