EXAMPLE_BIN= udptunnel

include ../Makefile.conf


udptunnel: Intermediate/udptunnel.o
	$(CXX) Intermediate/udptunnel.o -o udptunnel -lclanNetwork


static: Intermediate/udptunnel.o
	$(CXX) -static Intermediate/udptunnel.o -o mouse -lclanNetwork $(LINK_STATIC)
