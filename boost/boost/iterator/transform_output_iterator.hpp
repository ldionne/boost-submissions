// license here

#ifndef BOOST_ITERATOR_TRANSFORM_OUTPUT_ITERATOR_HPP
#define BOOST_ITERATOR_TRANSFORM_OUTPUT_ITERATOR_HPP

#include <boost/iterator/iterator_categories.hpp>


namespace boost {

/**
 * An output iterator applying a function to its pointee before chaining
 * it to another output iterator.
 */
template <typename F, typename OutputIterator>
class transform_output_iterator {
    F f_;
    OutputIterator out_;

    template <typename F_, typename OutputIterator_>
    class output_proxy {
        F_& f_;
        OutputIterator_& out_;

    public:
        explicit output_proxy(F_& f, OutputIterator_& out)
            : f_(f), out_(out)
        { }

        template <typename T>
        output_proxy& operator=(T const& x) {
            *out_ = f_(x);
            return *this;
        }
    };

public:
    typedef void value_type;
    typedef void reference;
    typedef void pointer;
    typedef void difference_type;
    typedef incrementable_traversal_tag iterator_category;

    template <typename G>
    transform_output_iterator<F,transform_output_iterator<G, OutputIterator> >
    and_then(G const& g) const {
        typedef transform_output_iterator<G, OutputIterator> aux;
        return transform_output_iterator<F, aux>(f_, aux(g, out_));
    }

    explicit transform_output_iterator(F const& f,
                                       OutputIterator const& iterator)
        : f_(f), out_(iterator)
    { }

    explicit transform_output_iterator(OutputIterator const& iterator)
        : out_(iterator)
    { }

    transform_output_iterator& operator++() {
        ++out_;
        return *this;
    }

    transform_output_iterator operator++(int) {
        transform_output_iterator tmp(*this);
        ++out_;
        return tmp;
    }

    output_proxy<F, OutputIterator> operator*() {
        return output_proxy<F, OutputIterator>(f_, out_);
    }

    output_proxy<F const, OutputIterator const> operator*() const {
        return output_proxy<F const, OutputIterator const>(f_, out_);
    }
};

template <typename Function, typename OutputIterator>
transform_output_iterator<Function, OutputIterator>
make_transform_output_iterator(Function const& f, OutputIterator const& out)
{ return transform_output_iterator<Function, OutputIterator>(f, out); }

} // end namespace boost

#endif // !BOOST_ITERATOR_TRANSFORM_OUTPUT_ITERATOR_HPP
