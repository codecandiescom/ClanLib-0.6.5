EXAMPLE_BIN=netbrowse

include ../Makefile.conf

OBJF	=	Intermediate/netbrowse.o \
       		Intermediate/serverwindow.o \
		Intermediate/serverlistwindow.o

netbrowse: $(OBJF)
	$(CXX) ${CXXFLAGS} -g $(OBJF) -o netbrowse -lclanCore -lclanApp -lclanDisplay -lclanGUI -lclanNetwork
