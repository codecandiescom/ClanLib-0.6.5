EXAMPLE_BIN=inputdevices

include ../Makefile.conf

inputdevices: Intermediate/input_devices.o
	$(CXX) Intermediate/input_devices.o -o input_devices -lclanCore -lclanApp -lclanDisplay

static: Intermediate/input_devices.o
	$(CXX) Intermediate/input_devices.o -o input_devices -lclanApp -lclanDisplay -lclanCore $(LINK_STATIC)
