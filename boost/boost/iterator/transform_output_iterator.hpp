// license here

#ifndef BOOST_ITERATOR_TRANSFORM_OUTPUT_ITERATOR_HPP
#define BOOST_ITERATOR_TRANSFORM_OUTPUT_ITERATOR_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost {

template <typename Iterator> struct is_transform_output_iterator_type;

/**
 * An output iterator applying a function to its pointee before chaining
 * it to another output iterator.
 */
template <typename UnaryFunction, typename Iterator>
class transform_output_iterator {
    UnaryFunction f_;
    Iterator out_;

    template <typename UnaryFunction_, typename Iterator_>
    class output_proxy {
        UnaryFunction_& f_;
        Iterator_& out_;

    public:
        explicit output_proxy(UnaryFunction_& f, Iterator_& out)
            : f_(f), out_(out)
        { }

        template <typename T>
        output_proxy& operator=(T const& x) {
            *out_ = f_(x);
            return *this;
        }
    };

    template <typename UnaryFunction_, typename Iterator_>
    friend class transform_output_iterator;

    template <typename It, typename F>
    struct apply_append {
        typedef typename It::template append<F>::type type;
    };

    template <typename G>
    struct append {
        typedef transform_output_iterator<UnaryFunction,
            typename mpl::eval_if<is_transform_output_iterator_type<Iterator>,
                apply_append<Iterator, G>,
                mpl::identity<transform_output_iterator<G, Iterator> >
            >::type
        > type;
    };

    template <typename G>
        typename enable_if<is_transform_output_iterator_type<Iterator>,
    typename append<G>::type>::type and_then_impl(G const& g) const {
        return typename append<G>::type(f_, out_.and_then_impl(g));
    }

    template <typename G>
        typename enable_if<mpl::not_<
                                is_transform_output_iterator_type<Iterator> >,
    typename append<G>::type>::type and_then_impl(G const& g) const {
        typedef transform_output_iterator<G, Iterator> last_type;
        typedef transform_output_iterator<UnaryFunction, last_type>
                                                            before_last_type;
        return before_last_type(f_, last_type(g, out_));
    }

public:
    typedef void value_type;
    typedef void reference;
    typedef void pointer;
    typedef void difference_type;
    typedef incrementable_traversal_tag iterator_category;

    template <typename G>
    typename append<G>::type and_then(G const& g = G()) const {
        return and_then_impl(g);
    }

    explicit transform_output_iterator(UnaryFunction const& f,
                                       Iterator const& iterator)
        : f_(f), out_(iterator)
    { }

    explicit transform_output_iterator(Iterator const& iterator)
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

    output_proxy<UnaryFunction, Iterator> operator*() {
        return output_proxy<UnaryFunction, Iterator>(f_, out_);
    }

    output_proxy<UnaryFunction const, Iterator const> operator*() const {
        return output_proxy<UnaryFunction const, Iterator const>(f_, out_);
    }
};

template <typename Iterator>
struct is_transform_output_iterator_type
    : false_type
{ };

template <typename UnaryFunction, typename Iterator>
struct is_transform_output_iterator_type<
                        transform_output_iterator<UnaryFunction, Iterator> >
    : true_type
{ };

template <typename Iterator>
bool is_transform_output_iterator(Iterator const&) {
    return ::boost::is_transform_output_iterator_type<Iterator>::value;
}

template <typename UnaryFunction, typename Iterator>
transform_output_iterator<UnaryFunction, Iterator>
make_transform_output_iterator(UnaryFunction const& f,
                               Iterator const& iterator) {
    return transform_output_iterator<UnaryFunction, Iterator>(f, iterator);
}

template <typename UnaryFunction, typename Iterator>
transform_output_iterator<UnaryFunction, Iterator>
make_transform_output_iterator(Iterator const& iterator) {
    return transform_output_iterator<UnaryFunction, Iterator>(iterator);
}

} // end namespace boost

#endif // !BOOST_ITERATOR_TRANSFORM_OUTPUT_ITERATOR_HPP
