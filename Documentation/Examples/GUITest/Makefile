EXAMPLE_BIN=dynamic

include ../Makefile.conf

TARGET = dynamic

DEBUGGER = medic
EDITOR = jcc
EDITOR_FILE = GUITest.jcc

current: $(TARGET)
	@echo
	@echo Note: This GUI is under development, and only one example
	@echo '      was built. To build all examples, do a "make all"'.


dynamic: Intermediate/dynamic.o
	$(CXX) -g Intermediate/dynamic.o -o dynamic -lclanGUI -lclanApp -lclanDisplay

labeltest: Intermediate/labeltest.o
	$(CXX) -g Intermediate/labeltest.o -o labeltest -lclanGUI -lclanApp

buttontest: Intermediate/buttontest.o
	$(CXX) -g Intermediate/buttontest.o -o buttontest -lclanGUI -lclanApp

frametest: Intermediate/frametest.o
	$(CXX) -g Intermediate/frametest.o -o frametest -lclanGUI -lclanApp

inputboxtest: Intermediate/inputboxtest.o
	$(CXX) -g Intermediate/inputboxtest.o -o inputboxtest -lclanGUI -lclanApp

run:
	./$(TARGET)

rundebug:
	$(DEBUGGER) ./$(TARGET)

edit:
	$(EDITOR) $(EDITOR_FILE) &

dep:
	@echo "  Building dependencies..."

	@find -name '*.cpp' | \
	sed "s/^.\///;" | \
	xargs $(CXX) -M | \
	sed "s/\(.*\.o:\)/Intermediate\/\1/g;" > Makefile.dep

#include Makefile.dep
