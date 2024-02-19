CXX = g++
CXXFLAGS = -std=c++2a -Wall -g -MMD
SRC_DIRS = . graphics players pieces

# Use wildcard and foreach to generate a list of source files
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cc))
OBJECTS = $(SRCS:.cc=.o)
DEPENDS = ${OBJECTS:.o=.d}
EXEC = chess



# First target in the makefile is the default target.
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) -lX11

%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS) -lX11

-include ${DEPENDS}

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DEPENDS) $(EXEC)
