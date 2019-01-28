EXAMPLE_BIN=jpeg

include ../Makefile.conf

jpeg: jpegtest.cpp
	@echo "  Compiling $<..."
	$(CXX) jpegtest.cpp -o jpegtest -lclanCore -lclanJPEG -lclanApp -lclanDisplay

static: Intermediate/jpegtest.o
	$(CXX) -static jpegtest.cpp -o jpegtest -lclanJPEG -lclanApp -lclanDisplay -lclanCore -ljpeg $(LINK_STATIC)
