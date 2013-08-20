// (C) Copyright 2013 Louis Dionne
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*!
 * @file
 * This file contains unit tests `boost::algorithm::is_cyclic_permutation`.
 */

#include <boost/algorithm/is_cyclic_permutation.hpp>

#include <boost/assert.hpp>
#include <boost/assign.hpp>
#include <functional>
#include <list>
#include <string>
#include <vector>


namespace {
using boost::assign::list_of;
using boost::algorithm::is_cyclic_permutation;

template <typename T>
void diff_size_ranges_are_not_cyclic_perms() {
    T s1 = list_of('a')('b')('c')('d'), s2 = list_of('a');
    BOOST_ASSERT(!is_cyclic_permutation(s1, s2));
    BOOST_ASSERT(!is_cyclic_permutation(s2, s1));
}

template <typename T>
void behaves_well_on_half_empty_input() {
    T s1 = list_of('a')('b')('c')('d'), s2;
    BOOST_ASSERT(!is_cyclic_permutation(s1, s2));
    BOOST_ASSERT(!is_cyclic_permutation(s2, s1));
}

template <typename T>
void behaves_well_on_completely_empty_input() {
    T s1, s2;
    BOOST_ASSERT(is_cyclic_permutation(s1, s2));
    BOOST_ASSERT(is_cyclic_permutation(s2, s1));
}

template <typename T>
void catches_left_shifted_by_one() {
    T s1 = list_of('a')('b')('c')('d')('e')('f'),
      s2 = list_of('b')('c')('d')('e')('f')('a');
    BOOST_ASSERT(is_cyclic_permutation(s1, s2));
    BOOST_ASSERT(is_cyclic_permutation(s2, s1));
}

template <typename T>
void catches_right_shifted_by_one() {
    T s1 = list_of('a')('b')('c')('d')('e')('f'),
      s2 = list_of('f')('a')('b')('c')('d')('e');
    BOOST_ASSERT(is_cyclic_permutation(s1, s2));
    BOOST_ASSERT(is_cyclic_permutation(s2, s1));
}

template <typename T>
void catches_when_equal_input() {
    T s1 = list_of('a')('b')('c')('d')('e')('f'),
      s2 = list_of('a')('b')('c')('d')('e')('f');
    BOOST_ASSERT(is_cyclic_permutation(s1, s2));
    BOOST_ASSERT(is_cyclic_permutation(s2, s1));
}

template <typename T>
void behaves_well_with_duplicate_values() {
    T s1 = list_of('a')('b')('a')('b')('a')('b')('c')('d'),
      s2 = list_of('b')('a')('b')('a')('b')('c')('d')('a');
    BOOST_ASSERT(is_cyclic_permutation(s1, s2));
    BOOST_ASSERT(is_cyclic_permutation(s2, s1));
}

template <typename T>
void behaves_well_with_shifts_larger_than_one() {
    T s1 = list_of('a')('b')('c')('d')('e')('f'),
      s2 = list_of('e')('f')('a')('b')('c')('d');
    BOOST_ASSERT(is_cyclic_permutation(s1, s2));
    BOOST_ASSERT(is_cyclic_permutation(s2, s1));
}

template <typename T>
void unrelated_strings_are_not_cyclic_perms() {
    T s1 = list_of('a')('b')('c')('d')('e')('f'),
      s2 = list_of('e')('f')('g')('h')('i')('j');
    BOOST_ASSERT(!is_cyclic_permutation(s1, s2));
    BOOST_ASSERT(!is_cyclic_permutation(s2, s1));
}

template <typename T>
void check_with_iterators() {
    T s1 = list_of('a')('b')('c')('d')('e')('f'),
      s2 = list_of('e')('f')('a')('b')('c')('d');
    BOOST_ASSERT(is_cyclic_permutation(s1.begin(), s1.end(),
                                       s2.begin(), s2.end()));
    BOOST_ASSERT(is_cyclic_permutation(s2.begin(), s2.end(),
                                       s1.begin(), s1.end()));
}

template <typename T>
void use_custom_predicate() {
    T s1 = list_of('a')('b')('c')('d')('e')('f'),
      s2 = list_of('e')('f')('a')('b')('c')('d');
    BOOST_ASSERT(is_cyclic_permutation(s1, s2, std::equal_to<char>()));
    BOOST_ASSERT(is_cyclic_permutation(s2, s1, std::equal_to<char>()));
}

template <typename T>
void test() {
    diff_size_ranges_are_not_cyclic_perms<T>();
    behaves_well_on_half_empty_input<T>();
    behaves_well_on_completely_empty_input<T>();
    catches_left_shifted_by_one<T>();
    catches_right_shifted_by_one<T>();
    catches_when_equal_input<T>();
    behaves_well_with_duplicate_values<T>();
    behaves_well_with_shifts_larger_than_one<T>();
    unrelated_strings_are_not_cyclic_perms<T>();
    check_with_iterators<T>();
    use_custom_predicate<T>();
}
} // end anonymous namespace

int main() {
    test<std::list<char> >();
    test<std::string>();
    test<std::vector<char> >();
}
