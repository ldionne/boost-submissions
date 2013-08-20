
project(boost_submissions CXX)
cmake_minimum_required(VERSION 2.8)

enable_testing()

find_package(Boost 1.53 REQUIRED)

add_custom_target(unit COMMENT "build all the unit tests")
add_custom_target(check
    DEPENDS unit
    COMMAND ${CMAKE_CTEST_COMMAND}
    COMMENT "build and then run all the unit tests"
)

function(add_test_ name sources)
    add_executable(${name} EXCLUDE_FROM_ALL ${sources})
    set_property(TARGET ${name} APPEND PROPERTY INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
    set_property(TARGET ${name} APPEND PROPERTY INCLUDE_DIRECTORIES ${boost_submissions_SOURCE_DIR}/boost)
    add_test(NAME ${name} COMMAND ${name})
    add_dependencies(unit ${name})
endfunction()

add_test_(is_cyclic_permutation boost/libs/algorithm/test/is_cyclic_permutation_test.cpp)

add_test_(accessor_iterator boost/libs/iterator/test/accessor_iterator_test.cpp)
add_test_(transform_output_iterator boost/libs/iterator/test/transform_output_iterator_test.cpp)

add_test_(accessor_property_map boost/libs/property_map/test/accessor_property_map_test.cpp)
add_test_(chained_property_map boost/libs/property_map/test/chained_property_map_test.cpp)
add_test_(property_traits boost/libs/property_map/test/property_traits_test.cpp)
