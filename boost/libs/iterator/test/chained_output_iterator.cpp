// license here

#include <algorithm>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/iterator/chained_output_iterator.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <cassert>
#include <iterator>
#include <vector>


namespace {

template <typename T>
struct Identity {
    typedef T result_type;
    result_type operator()(T t) { return t; }
};

BOOST_CONCEPT_ASSERT((boost::OutputIterator<
                        boost::chained_output_iterator<
                            Identity<int>,
                            std::back_insert_iterator<std::vector<int> >
                        >,
                        int
                    >));

BOOST_CONCEPT_ASSERT((boost_concepts::WritableIterator<
                        boost::chained_output_iterator<
                            Identity<int>,
                            std::back_insert_iterator<std::vector<int> >
                        >,
                        int
                    >));

BOOST_CONCEPT_ASSERT((boost_concepts::IncrementableIterator<
                        boost::chained_output_iterator<
                            Identity<int>,
                            std::back_insert_iterator<std::vector<int> >
                        >
                    >));

template <int n>
struct Multiply {
    typedef int result_type;
    result_type operator()(int val) const {
        return val * n;
    }
};

template <int n>
struct Add {
    typedef int result_type;
    result_type operator()(int val) const {
        return val + n;
    }
};

typedef std::vector<int> Vector;
typedef std::back_insert_iterator<Vector> BackInserter;
template <int n>
struct AddingBackInserter {
    typedef boost::chained_output_iterator<Add<n>, BackInserter> type;
};


void should_transform_output() {
    Vector vec;
    std::copy(boost::counting_iterator<int>(0),
              boost::counting_iterator<int>(10),
              AddingBackInserter<10>::type(BackInserter(vec)));

    Vector expected;
    std::copy(boost::counting_iterator<int>(10),
              boost::counting_iterator<int>(20),
              BackInserter(expected));

    assert(vec == expected);
}

void test_with_deduction() {
    Vector vec;
    AddingBackInserter<10>::type out =
        boost::make_chained_output_iterator(Add<10>(), BackInserter(vec));

    std::copy(boost::counting_iterator<int>(0),
              boost::counting_iterator<int>(10),
              out);

    Vector expected;
    std::copy(boost::counting_iterator<int>(10),
              boost::counting_iterator<int>(20),
              BackInserter(expected));

    assert(vec == expected);
}

void should_allow_chains() {
    Vector vec;
    typedef AddingBackInserter<10>::type Add10;
    typedef boost::chained_output_iterator<Multiply<2>, Add10> Mul2Plus10;
    std::copy(boost::counting_iterator<int>(0),
              boost::counting_iterator<int>(5),
              Mul2Plus10(Add10(BackInserter(vec))));

    Vector expected;
    expected.push_back(0 * 2 + 10);
    expected.push_back(1 * 2 + 10);
    expected.push_back(2 * 2 + 10);
    expected.push_back(3 * 2 + 10);
    expected.push_back(4 * 2 + 10);

    assert(vec == expected);
}

void should_allow_construction_across_chain() {
    typedef boost::chained_output_iterator<Multiply<2>,
                boost::chained_output_iterator<Add<3>,
                    std::back_insert_iterator<Vector> > > Midway;
    typedef boost::chained_output_iterator<Add<5>, Midway> OutputIter;
    Vector vec;
    OutputIter out1(std::back_inserter(vec));
    *out1 = 1;

    OutputIter out2(Midway(std::back_inserter(vec)));
    *out2 = 2;

    assert(vec.front() == (1 + 5) * 2 + 3);
    assert(vec.back() == (2 + 5) * 2 + 3);
}

void should_work_for_const_iterator_if_wrapped_iter_has_const_deref() {
    typedef boost::chained_output_iterator<Multiply<2>,
                Vector::iterator> OutputIter;
    Vector vec(1);
    OutputIter const out(vec.begin());
    *out = 3;

    assert(vec.front() == 2 * 3);
}

} // end anonymous namespace


int main(int, char const*[]) {
    should_transform_output();
    test_with_deduction();
    should_allow_chains();
    should_allow_construction_across_chain();
    should_work_for_const_iterator_if_wrapped_iter_has_const_deref();
    return 0;
}
