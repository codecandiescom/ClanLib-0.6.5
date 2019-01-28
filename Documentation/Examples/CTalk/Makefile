
EXAMPLE_BIN=    CTalk

include ../Makefile.conf

OBJF= \
      	Intermediate/canvas.o \
	Intermediate/canvas_view.o \
	Intermediate/chat_component.o \
	Intermediate/chat_view.o \
	Intermediate/ctalk.o \
	Intermediate/document.o \
	Intermediate/document_server.o \
	Intermediate/input_label.o \
	Intermediate/login_view.o \
	Intermediate/mainframe.o \
	Intermediate/sketch.o \
	Intermediate/userlist_component.o \
	Intermediate/view.o

CTalk: $(OBJF)
	$(CXX) $(OBJF) -o ctalk_example -lclanCore -lclanApp -lclanDisplay -lclanGUI -lclanNetwork
