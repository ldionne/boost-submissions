// (C) Copyright 2013 Louis Dionne
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*!
 * @file
 * This file defines `boost::algorithm::is_cyclic_permutation`.
 */

#ifndef BOOST_ALGORITHM_IS_CYCLIC_PERMUTATION_HPP
#define BOOST_ALGORITHM_IS_CYCLIC_PERMUTATION_HPP

#include <algorithm>
#include <boost/assert.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/move/utility.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/next_prior.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/phoenix/core/argument.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/join.hpp>
#include <boost/regex/v4/iterator_category.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <iterator>
#include <utility>


namespace boost { namespace algorithm {
namespace is_cyclic_permutation_detail {
/**
 * @internal
 * Tag used when we know that we're comparing ranges of the same length.
 */
struct ranges_have_the_same_length { };

//! @internal Tag used when we can't assume both ranges have the same length.
struct dont_know_if_same_length { };

/**
 * @internal
 * Tag used when the predicate to compare elements is the default `operator==`.
 */
struct use_operator_equal { };

//! @internal Dispatch to the right implementation of `std::equal`.
template <
    typename Iterator1, typename Iterator1x,
    typename Iterator2, typename BinaryPredicate
>
bool std_equal_dispatch(BOOST_FWD_REF(Iterator1) first1,
                        BOOST_FWD_REF(Iterator1x) last1,
                        BOOST_FWD_REF(Iterator2) first2,
                        BOOST_FWD_REF(BinaryPredicate) pred) {
    return std::equal(boost::forward<Iterator1>(first1),
                      boost::forward<Iterator1x>(last1),
                      boost::forward<Iterator2>(first2),
                      boost::forward<BinaryPredicate>(pred));
}

template <typename Iterator1, typename Iterator1x, typename Iterator2>
bool std_equal_dispatch(BOOST_FWD_REF(Iterator1) first1,
                        BOOST_FWD_REF(Iterator1x) last1,
                        BOOST_FWD_REF(Iterator2) first2,
                        use_operator_equal) {
    return std::equal(boost::forward<Iterator1>(first1),
                      boost::forward<Iterator1x>(last1),
                      boost::forward<Iterator2>(first2));
}

//! @internal Dispatch to the right implementation of `std::mismatch`.
template <
    typename Iterator1, typename Iterator1x,
    typename Iterator2, typename BinaryPredicate
>
std::pair<
    typename remove_reference<Iterator1>::type,
    typename remove_reference<Iterator2>::type
> std_mismatch_dispatch(BOOST_FWD_REF(Iterator1) first1,
                        BOOST_FWD_REF(Iterator1x) last1,
                        BOOST_FWD_REF(Iterator2) first2,
                        BOOST_FWD_REF(BinaryPredicate) pred) {
    return std::mismatch(boost::forward<Iterator1>(first1),
                         boost::forward<Iterator1x>(last1),
                         boost::forward<Iterator2>(first2),
                         boost::forward<BinaryPredicate>(pred));
}

template <typename Iterator1, typename Iterator1x, typename Iterator2>
std::pair<
    typename remove_reference<Iterator1>::type,
    typename remove_reference<Iterator2>::type
> std_mismatch_dispatch(BOOST_FWD_REF(Iterator1) first1,
                        BOOST_FWD_REF(Iterator1x) last1,
                        BOOST_FWD_REF(Iterator2) first2,
                        use_operator_equal) {
    return std::mismatch(boost::forward<Iterator1>(first1),
                         boost::forward<Iterator1x>(last1),
                         boost::forward<Iterator2>(first2));
}

/**
 * @internal
 * Dispatch to the right implementation of `std::find` or `std::find_if`.
 */
template <
    typename Iterator, typename Iteratorx,
    typename Value, typename BinaryPredicate
>
typename remove_reference<Iterator>::type
std_find_if_dispatch(BOOST_FWD_REF(Iterator) first,
                     BOOST_FWD_REF(Iteratorx) last,
                     BOOST_FWD_REF(Value) value,
                     BOOST_FWD_REF(BinaryPredicate) pred) {
    return std::find_if(boost::forward<Iterator>(first),
                        boost::forward<Iteratorx>(last),
                        phoenix::bind(
                            boost::forward<BinaryPredicate>(pred),
                                boost::forward<Value>(value),
                                phoenix::arg_names::_1));
}

template <typename Iterator, typename Iteratorx, typename Value>
typename remove_reference<Iterator>::type
std_find_if_dispatch(BOOST_FWD_REF(Iterator) first,
                     BOOST_FWD_REF(Iteratorx) last,
                     BOOST_FWD_REF(Value) value, use_operator_equal) {
    return std::find(boost::forward<Iterator>(first),
                     boost::forward<Iteratorx>(last),
                     boost::forward<Value>(value));
}

/**
 * @internal
 * Metafunction returning whether it is O(1) to compute the distance between
 * two iterators.
 */
template <typename Iterator>
struct has_O1_distance
    : mpl::bool_<
        ::boost::is_random_access_iterator<Iterator>::value
    >
{ };

/**
 * @internal
 * Perform a comparison between a range and another shifted range.
 * This version is used when we use the default operator for comparison.
 */
template <typename Iterator1, typename Iterator2>
bool compare_shifted(Iterator1 first1, Iterator1 last1,
                     Iterator2 first2, Iterator2 shift, Iterator2 last2,
                     use_operator_equal, dont_know_if_same_length) {
    // return whether [first1, last1) == [shift, last2) U [first2, shift)
    return equal(
            make_iterator_range(first1, last1),

            join(
                make_iterator_range(shift, last2),
                make_iterator_range(first2, shift)));
}

/**
 * @internal
 * Perform a comparison between a range and another shifted range.
 * This version is used when we use a custom predicate for comparison.
 */
template <typename Iterator1, typename Iterator2, typename BinaryPredicate>
bool compare_shifted(Iterator1 first1, Iterator1 last1,
                     Iterator2 first2, Iterator2 shift, Iterator2 last2,
                     BOOST_FWD_REF(BinaryPredicate) pred,
                     dont_know_if_same_length) {
    // return whether [first1, last1) == [shift, last2) U [first2, shift)
    return equal(
            make_iterator_range(first1, last1),

            join(
                make_iterator_range(shift, last2),
                make_iterator_range(first2, shift)),

            boost::forward<BinaryPredicate>(pred));
}

/**
 * @internal
 * Overload of `compare_shifted` for when we know both ranges have
 * the same length.
 */
template <typename Iterator1, typename Iterator2, typename BinaryPredicate>
bool compare_shifted(Iterator1 first1, Iterator1 /* last1 */,
                     Iterator2 first2, Iterator2 shift, Iterator2 last2,
                     BinaryPredicate pred, ranges_have_the_same_length) {
    std::pair<Iterator2, Iterator1> const& mism =
                            std_mismatch_dispatch(shift, last2, first1, pred);
    // Let n = distance(shift, last2).
    // We return whether the two following conditions are met:
    //  (1) [shift, last2) == [first1, first1 + n)
    //  (2) [first2, shift) == [first1 + n, last1)
    return mism.first == last2 &&
           std_equal_dispatch(first2, shift, mism.second, pred);
}

//! @internal Core of the algorithm working on non empty ranges.
template <typename Iterator1, typename Iterator2,
          typename BinaryPredicate, typename LengthKnowledge>
bool is_nonempty_cyclic_permutation(Iterator1 first1, Iterator1 last1,
                                    Iterator2 first2, Iterator2 last2,
                                    BinaryPredicate pred, LengthKnowledge) {
    BOOST_ASSERT_MSG(first1 != last1, "[first1, last1) must be non-empty");
    BOOST_ASSERT_MSG(first2 != last2, "[first2, last2) must be non-empty");

    Iterator2 shift = std_find_if_dispatch(first2, last2, *first1, pred);
    while (shift != last2) {
        if (compare_shifted(first1, last1,
                            first2, shift, last2,
                            pred, LengthKnowledge()))
            return true;
        else
            shift = std_find_if_dispatch(boost::next(shift), last2,
                                         *first1, pred);
    }
    return false;
}

/**
 * Algorithm returning whether a range delimited by [`first1`, `last1`) is
 * a cyclic permutation of the range delimited by [`first2`, `last2`).
 *
 * The comparison between elements of the ranges is done using `operator==`.
 *
 * If both ranges are empty, the algorithm returns `true`.
 */
template <
    typename Iterator1, typename Iterator1x,
    typename Iterator2, typename Iterator2x
>
bool is_cyclic_permutation(BOOST_FWD_REF(Iterator1) first1,
                           BOOST_FWD_REF(Iterator1x) last1,
                           BOOST_FWD_REF(Iterator2) first2,
                           BOOST_FWD_REF(Iterator2x) last2) {
    return is_cyclic_permutation(boost::forward<Iterator1>(first1),
                                 boost::forward<Iterator1x>(last1),
                                 boost::forward<Iterator2>(first2),
                                 boost::forward<Iterator2x>(last2),
                                 use_operator_equal());
}

/**
 * Version of `is_cyclic_permutation` using `BinaryPredicate` to determine the
 * equality of two elements.
 *
 * @internal This version is optimized for random access iterators.
 */
template <typename Iterator1, typename Iterator2, typename BinaryPredicate>
typename enable_if<
    mpl::and_<
        has_O1_distance<Iterator1>, has_O1_distance<Iterator2>
    >,
bool>::type is_cyclic_permutation(Iterator1 first1, Iterator1 last1,
                                  Iterator2 first2, Iterator2 last2,
                                  BOOST_FWD_REF(BinaryPredicate) pred) {
    typedef typename iterator_difference<Iterator1>::type Distance1;
    typedef typename iterator_difference<Iterator2>::type Distance2;

    using std::distance;
    Distance1 const dist1 = distance(first1, last1);
    Distance2 const dist2 = distance(first2, last2);

    if (dist1 != dist2)
        return false;
    // dist2 == 0 need not be checked because dist1 == dist2
    else if (dist1 == 0)
        return true;
    else
        return is_nonempty_cyclic_permutation(first1, last1, first2, last2,
                                        boost::forward<BinaryPredicate>(pred),
                                        ranges_have_the_same_length());
}

/**
 * @internal
 * Version of `is_cyclic_permutation` for non random access iterators.
 */
template <typename Iterator1, typename Iterator2, typename BinaryPredicate>
typename disable_if<
    mpl::and_<
        has_O1_distance<Iterator1>, has_O1_distance<Iterator2>
    >,
bool>::type is_cyclic_permutation(Iterator1 first1, Iterator1 last1,
                                  Iterator2 first2, Iterator2 last2,
                                  BOOST_FWD_REF(BinaryPredicate) pred) {
    // [first1, last1) is empty
    if (first1 == last1)
        return first2 == last2;
    // [first2, last2) is empty, but [first1, last1) is not
    else if (first2 == last2)
        return false;
    else
        return is_nonempty_cyclic_permutation(first1, last1, first2, last2,
                                        boost::forward<BinaryPredicate>(pred),
                                        dont_know_if_same_length());
}

//! Overload of `is_cyclic_permutation` taking ranges.
template <typename Range1, typename Range2, typename BinaryPredicate>
bool is_cyclic_permutation(Range1 const& range1, Range2 const& range2,
                           BOOST_FWD_REF(BinaryPredicate) pred) {
    return is_cyclic_permutation(boost::begin(range1), boost::end(range1),
                                 boost::begin(range2), boost::end(range2),
                                 boost::forward<BinaryPredicate>(pred));
}

template <typename Range1, typename Range2>
bool is_cyclic_permutation(BOOST_FWD_REF(Range1) range1,
                           BOOST_FWD_REF(Range2) range2) {
    return is_cyclic_permutation(boost::forward<Range1>(range1),
                                 boost::forward<Range2>(range2),
                                 use_operator_equal());
}
} // end namespace is_cyclic_permutation_detail

using is_cyclic_permutation_detail::is_cyclic_permutation;
}} // end namespace boost::algorithm

#endif // !BOOST_ALGORITHM_IS_CYCLIC_PERMUTATION_HPP
