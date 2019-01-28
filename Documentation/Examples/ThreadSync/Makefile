EXAMPLE_BIN=    threadsync

include ../Makefile.conf

threadsync: Intermediate/tsync.o
	$(CXX) $(CPPFLAGS) $(LDFLAGS) Intermediate/tsync.o -o tsync -lclanCore -lclanApp