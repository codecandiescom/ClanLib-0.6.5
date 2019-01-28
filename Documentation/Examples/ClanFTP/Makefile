EXAMPLE_BIN=    clftp

include ../Makefile.conf


CFLAGS=-o -g3

LDFLAGS=-lclanCore -lclanNetwork -lclanApp

clftp: main.cpp ftp.cpp ftp.h
	$(CXX) -g3 -o clftp main.cpp ftp.cpp  $(LDFLAGS) -DDEBUG
