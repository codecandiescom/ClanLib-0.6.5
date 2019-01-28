EXAMPLE_BIN= gllinedemo

include ../Makefile.conf

gllinedemo: Intermediate/app.o Intermediate/line.o
	$(CXX) -g Intermediate/app.o Intermediate/line.o -o gllinedemo -ldl -lclanCore -lclanApp -lclanGL -lclanPNG -lclanDisplay
