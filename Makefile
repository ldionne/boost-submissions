CXX ?= c++
CXXFLAGS += -Wall -Wextra -pedantic -I `pwd`/boost
LDFLAGS +=

.PHONY: clean mrproper

all: accessor_iterator property_traits accessor_property_map \
	 chained_output_iterator chained_property_map accessor_iterator_example

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


chained_output_iterator: build/chained_output_iterator.o
	${CXX} -o build/$@ $^ ${LDFLAGS}

build/chained_output_iterator.o: boost/libs/iterator/test/chained_output_iterator.cpp \
								 boost/boost/iterator/chained_output_iterator.hpp
	${CXX} -o $@ -c $< ${CXXFLAGS}


chained_property_map: build/chained_property_map.o
	${CXX} -o build/$@ $^ ${LDFLAGS}

build/chained_property_map.o: boost/libs/property_map/test/chained_property_map.cpp \
							  boost/boost/property_map/chained_property_map.hpp
	${CXX} -o $@ -c $< ${CXXFLAGS}


accessor_iterator_example: build/accessor_iterator_example.o
	${CXX} -o build/$@ $^ ${LDFLAGS}

build/accessor_iterator_example.o: boost/libs/iterator/example/accessor_iterator_example.cpp
	${CXX} -o $@ -c $< ${CXXFLAGS}
