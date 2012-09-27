CXX ?= gcc
CXXFLAGS += -Wall -Wextra -pedantic -I `pwd`/boost
LDFLAGS +=

.PHONY: clean mrproper

clean:
	rm -rf build/*.o

mrproper:
	rm -rf build/*

all: accessor_iterator


accessor_iterator: build/accessor_iterator.o
	${CXX} -o build/$@ $^ ${LDFLAGS}

build/accessor_iterator.o: boost/libs/iterator/test/accessor_iterator.cpp \
						   boost/boost/iterator/accessor_iterator.hpp
	${CXX} -o $@ -c $< ${CXXFLAGS}
