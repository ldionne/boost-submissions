// license here

#ifndef BOOST_ITERATOR_CHAINED_OUTPUT_ITERATOR_HPP
#define BOOST_ITERATOR_CHAINED_OUTPUT_ITERATOR_HPP

#include <boost/iterator/iterator_categories.hpp>


namespace boost {

/**
 * An output iterator applying a function to its pointee before chaining
 * it to another output iterator.
 */
template <typename Function, typename OutputIterator>
class chained_output_iterator {
    Function f_;
    OutputIterator out_;

    template <typename Fun, typename It>
    struct OutputProxy {
        explicit OutputProxy(Fun& f, It& out)
            : f_(f), out_(out)
        { }

        template <typename T>
        OutputProxy& operator=(T const& x) {
            *out_ = f_(x);
            return *this;
        }

        Fun& f_;
        It& out_;
    };

public:
    typedef void value_type;
    typedef void reference;
    typedef void pointer;
    typedef void difference_type;
    typedef incrementable_traversal_tag iterator_category;

    explicit chained_output_iterator(Function const& f,
                                     OutputIterator const& iterator)
        : f_(f), out_(iterator)
    { }

    explicit chained_output_iterator(OutputIterator const& iterator)
        : out_(iterator)
    { }

    chained_output_iterator& operator++() {
        ++out_;
        return *this;
    }

    chained_output_iterator operator++(int) {
        chained_output_iterator tmp(*this);
        ++out_;
        return tmp;
    }

    OutputProxy<Function, OutputIterator> operator*() {
        return OutputProxy<Function, OutputIterator>(f_, out_);
    }

    OutputProxy<Function const, OutputIterator const> operator*() const {
        return OutputProxy<Function const, OutputIterator const>(f_, out_);
    }
};

template <typename Function, typename OutputIterator>
chained_output_iterator<Function, OutputIterator>
make_chained_output_iterator(Function const& f, OutputIterator const& out)
{ return chained_output_iterator<Function, OutputIterator>(f, out); }

} // end namespace boost

#endif // !BOOST_ITERATOR_CHAINED_OUTPUT_ITERATOR_HPP
