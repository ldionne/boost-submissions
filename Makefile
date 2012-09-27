CXX ?= gcc
CXXFLAGS = -Wall -Wextra -pedantic
LDFLAGS =

.PHONY: clean

all:

%.o: %.cpp
	${CXX} -o $@ -c $< ${CXXFLAGS}

clean:
	rm -rf *.o
