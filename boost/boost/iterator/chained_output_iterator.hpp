// license here

#ifndef BOOST_ITERATOR_CHAINED_OUTPUT_ITERATOR_HPP
#define BOOST_ITERATOR_CHAINED_OUTPUT_ITERATOR_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost {

/**
 * An output iterator applying a function to its pointee before chaining
 * it to another output iterator.
 */
template <typename Function, typename OutputIterator>
class chained_output_iterator {
    template <typename From, typename To>
    struct enable_if_convertible : enable_if<is_convertible<From, To> > { };

    Function f_;
    OutputIterator out_;

    struct OutputProxy {
        explicit OutputProxy(Function& f, OutputIterator& out)
            : f_(f), out_(out)
        { }

        template <typename T>
        OutputProxy& operator=(T const& x) {
            *out_ = f_(x);
            ++out_;
            return *this;
        }

        Function& f_;
        OutputIterator& out_;
    };

public:
    typedef void value_type;
    typedef void reference;
    typedef void pointer;
    typedef void difference_type;
    typedef incrementable_traversal_tag iterator_category;

    explicit chained_output_iterator(Function const& f,
                                     OutputIterator const& out)
        : f_(f), out_(out)
    { }

    template <typename OutIter>
    chained_output_iterator(
        OutIter const& out,
        typename enable_if_convertible<OutIter, OutputIterator>::type* =0)
        : out_(out)
    { }

    template <typename Func, typename OutIter>
    chained_output_iterator(
        chained_output_iterator<Func, OutIter> const& other,
        typename enable_if_convertible<Func, Function>::type* =0,
        typename enable_if_convertible<OutIter, OutputIterator>::type* =0)
        : f_(other.f_), out_(other.out_)
    { }

    chained_output_iterator& operator++() { return *this; }
    chained_output_iterator& operator++(int) { return *this; }
    OutputProxy operator*() { return OutputProxy(f_, out_); }
};

template <typename Function, typename OutputIterator>
chained_output_iterator<Function, OutputIterator>
make_chained_output_iterator(Function const& f, OutputIterator const& out)
{ return chained_output_iterator<Function, OutputIterator>(f, out); }

} // end namespace boost

#endif // !BOOST_ITERATOR_CHAINED_OUTPUT_ITERATOR_HPP
