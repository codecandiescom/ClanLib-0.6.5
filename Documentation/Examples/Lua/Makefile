EXAMPLE_BIN= lua

include ../Makefile.conf

lua: Intermediate/lua_test.o
	@echo This example is known NOT TO WORK
	$(CXX) Intermediate/lua_test.o -o lua_test -lclanCore -lclanDisplay -lclanLua -lclanApp -llualib -llua
