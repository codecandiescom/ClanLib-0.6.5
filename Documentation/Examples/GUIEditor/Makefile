EXAMPLE_BIN=editor

include ../Makefile.conf

OBJF =	Intermediate/editor.o \
	Intermediate/component_node.o \
	Intermediate/component_type_window.o \
	Intermediate/create_component_window.o \
	Intermediate/document.o \
	Intermediate/editview.o \
	Intermediate/mainframe.o \
	Intermediate/gui_project_window.o \
	Intermediate/propertybar.o \
	Intermediate/toolbar.o \
	Intermediate/treebar.o

editor: $(OBJF)
	$(CXX) $(OBJF) -o editor -lclanCore -lclanApp -lclanDisplay -lclanGUI
