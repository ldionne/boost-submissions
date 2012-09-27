CXX ?= gcc
CXXFLAGS += -Wall -Wextra -pedantic -I `pwd`/boost
LDFLAGS +=

.PHONY: clean mrproper

all: accessor_iterator property_traits accessor_property_map

clean:
	rm -rf build/*.o

mrproper:
	rm -rf build/*


accessor_iterator: build/accessor_iterator.o
	${CXX} -o build/$@ $^ ${LDFLAGS}

build/accessor_iterator.o: boost/libs/iterator/test/accessor_iterator.cpp \
						   boost/boost/iterator/accessor_iterator.hpp
	${CXX} -o $@ -c $< ${CXXFLAGS}


property_traits: build/property_traits.o
	${CXX} -o build/$@ $^ ${LDFLAGS}

build/property_traits.o: boost/libs/property_map/test/property_traits.cpp \
						 boost/boost/property_map/property_traits.hpp
	${CXX} -o $@ -c $< ${CXXFLAGS}


accessor_property_map: build/accessor_property_map.o
	${CXX} -o build/$@ $^ ${LDFLAGS}

build/accessor_property_map.o: boost/libs/property_map/test/accessor_property_map.cpp \
							   boost/boost/property_map/accessor_property_map.hpp
	${CXX} -o $@ -c $< ${CXXFLAGS}
