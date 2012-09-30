// license here

#ifndef BOOST_ITERATOR_CHAINED_OUTPUT_ITERATOR_HPP
#define BOOST_ITERATOR_CHAINED_OUTPUT_ITERATOR_HPP

#include <boost/function_output_iterator.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost {

namespace detail {

template <typename Function, typename OutputIterator>
struct output_into_next {
    explicit output_into_next(Function const& f, OutputIterator const& out)
        : f_(f), out_(out) { }

    template <typename T>
    void operator()(T const& val) {
        *out_ = f_(val);
        ++out_;
    }

    Function f_;
    OutputIterator out_;
};

} // end namespace detail

/**
 * An output iterator applying a function to its pointee before chaining
 * it to another output iterator.
 */
template <typename Function, typename OutputIterator>
class chained_output_iterator
    : public function_output_iterator<
        detail::output_into_next<Function, OutputIterator> > {

    template <typename From, typename To>
    struct enable_if_convertible : enable_if<is_convertible<From, To> > { };

    typedef detail::output_into_next<Function, OutputIterator> OutputProxy;
    typedef function_output_iterator<OutputProxy> Base;

public:
    template <typename OutIter>
    explicit chained_output_iterator(
        Function const& f,
        OutIter const& out,
        typename enable_if_convertible<OutIter, OutputIterator>::type* =0)
        : Base(OutputProxy(f, out))
    { }

    template <typename OutIter>
    chained_output_iterator(
        OutIter const& out,
        typename enable_if_convertible<OutIter, OutputIterator>::type* =0)
        : Base(OutputProxy(Function(), out))
    { }
};

template <typename Function, typename OutputIterator>
chained_output_iterator<Function, OutputIterator>
make_chained_output_iterator(Function const& f, OutputIterator const& out)
{ return chained_output_iterator<Function, OutputIterator>(f, out); }

} // end namespace boost

#endif // !BOOST_ITERATOR_CHAINED_OUTPUT_ITERATOR_HPP
