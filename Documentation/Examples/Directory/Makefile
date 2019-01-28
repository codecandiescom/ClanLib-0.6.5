EXAMPLE_BIN=directory_test

include ../Makefile.conf

directory_test: directory_test.cpp
	@echo "  Compiling $<..."
	$(CXX) $< -o $@ -lclanCore -lclanJPEG -lclanApp -lclanDisplay

static: Intermediate/directory_test.o
	$(CXX) -static directory_test.cpp -o directory_test -lclanJPEG -lclanApp -lclanDisplay -lclanCore -ljpeg $(LINK_STATIC)
