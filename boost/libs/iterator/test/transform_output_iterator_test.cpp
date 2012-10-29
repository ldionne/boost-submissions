// Copyright Louis Dionne 2012. Use, modification and distribution is subject
// to the Boost Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/iterator/transform_output_iterator.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/concept_archetype.hpp>
#include <boost/concept_check.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <string>
#include <vector>


namespace transform_output_iterator_test {

template <typename Iterator, typename OutputType = typename
                                boost::iterator_value<Iterator>::type>
struct test_concepts {
    BOOST_CONCEPT_USAGE(test_concepts) {
        BOOST_CONCEPT_ASSERT((boost::OutputIterator<
                                boost::transform_output_iterator<
                                    OutputType (*)(OutputType),
                                    Iterator
                                >,
                                OutputType
                            >));

        BOOST_CONCEPT_ASSERT((boost_concepts::WritableIterator<
                                boost::transform_output_iterator<
                                    OutputType (*)(OutputType),
                                    Iterator
                                >,
                                OutputType
                            >));

        BOOST_CONCEPT_ASSERT((boost_concepts::IncrementableIterator<
                                boost::transform_output_iterator<
                                    OutputType (*)(OutputType),
                                    Iterator
                                >
                            >));
    }
};

struct dummy { };
BOOST_CONCEPT_ASSERT((
    test_concepts<boost::output_iterator_archetype<dummy>, dummy>
));
BOOST_CONCEPT_ASSERT((
    test_concepts<boost::input_output_iterator_archetype<dummy>, dummy>
));
BOOST_CONCEPT_ASSERT((
    test_concepts<boost::mutable_forward_iterator_archetype<dummy> >
));
BOOST_CONCEPT_ASSERT((
    test_concepts<boost::mutable_bidirectional_iterator_archetype<dummy> >
));
BOOST_CONCEPT_ASSERT((
    test_concepts<boost::mutable_random_access_iterator_archetype<dummy> >
));

template <typename T = int>
struct Multiply {
    T n_;
    Multiply(T n) : n_(n) { }
    T operator()(T val) const {
        return val * n_;
    }
};

template <typename T = int>
struct Add {
    T n_;
    Add(T n) : n_(n) { }
    T operator()(T val) const {
        return val + n_;
    }
};

void should_transform_output() {
    std::vector<int> actual, expected;
    std::copy(boost::counting_iterator<int>(0),
              boost::counting_iterator<int>(10),
              boost::make_transform_output_iterator(
                                    Add<>(10), std::back_inserter(actual)));

    std::copy(boost::counting_iterator<int>(10),
              boost::counting_iterator<int>(20),
              std::back_inserter(expected));

    assert(actual == expected);
}

void should_allow_chaining() {
    std::vector<int> actual, expected;

    boost::transform_output_iterator<Multiply<>,
        boost::transform_output_iterator<Add<>,
            std::back_insert_iterator<std::vector<int> > > > output =
            boost::make_transform_output_iterator(
                Multiply<>(2),std::back_inserter(actual)).and_then(Add<>(10));

    std::copy(boost::counting_iterator<int>(0),
              boost::counting_iterator<int>(5),
              output);

    expected.push_back(0 * 2 + 10);
    expected.push_back(1 * 2 + 10);
    expected.push_back(2 * 2 + 10);
    expected.push_back(3 * 2 + 10);
    expected.push_back(4 * 2 + 10);

    assert(actual == expected);
}

void should_work_for_const_iterator_if_wrapped_iter_has_const_deref() {
    typedef boost::transform_output_iterator<Multiply<>,
                std::vector<int>::iterator> OutputIter;
    std::vector<int> actual(1), expected;
    OutputIter const out(Multiply<>(2), actual.begin());
    *out = 3;

    expected.push_back(2 * 3);

    assert(actual == expected);
}

void chain_performs_operation_in_right_order() {
    std::string actual;
    typedef Add<std::string> Append;
    *boost::make_transform_output_iterator(Append("a"), &actual).
                                  and_then(Append("b"))
                                 .and_then(Append("c"))
                                 .and_then(Append("d")) = "";

    assert(actual == "abcd");
}

} // end transform_output_iterator_test namespace


int main() {
    using namespace transform_output_iterator_test;
    should_transform_output();
    should_allow_chaining();
    should_work_for_const_iterator_if_wrapped_iter_has_const_deref();
    chain_performs_operation_in_right_order();
    return 0;
}
