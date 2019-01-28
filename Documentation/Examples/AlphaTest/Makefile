# AlphaTest

EXAMPLE_BIN=alphatest

include ../Makefile.conf

#the bin targets

alphatest: Intermediate/alphatest.o
	$(CXX) ${CXXFLAGS} Intermediate/alphatest.o -o alphatest -lclanCore -lclanApp -lclanJPEG -lclanDisplay

static: Intermediate/alphatest.o
	$(CXX) -static ${CXXFLAGS} Intermediate/alphatest.o -o alphatest -lclanApp -lclanJPEG -ljpeg -lclanDisplay -lclanCore $(LINK_STATIC) 
