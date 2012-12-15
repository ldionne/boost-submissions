// (C) Copyright 2012 Louis Dionne
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ITERATOR_TRANSFORM_OUTPUT_ITERATOR_HPP
#define BOOST_ITERATOR_TRANSFORM_OUTPUT_ITERATOR_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/mpl/bool_fwd.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/decay.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost {

namespace detail {

template <typename Derived, typename Iterator>
class proxy_base {
    template <typename OutputProxy>
    friend class output_iterator;

    Iterator out_;

    Derived& derived() {
        return *static_cast<Derived*>(this);
    }

    Derived const& derived() const {
        return *static_cast<Derived const*>(this);
    }

    template <typename Proxy>
    ... combine_with(Proxy const& other) {

    }

protected:
    template <typename T>
    void do_output(T const& t) {
        *out_ = t;
    }

public:
    template <typename T>
    Derived& operator=(T const& x) {
        this->derived().on_assignment(x);
        return this->derived();
    }

    template <typename T>
    Derived const& operator=(T const& x) const {
        this->derived().on_assignment(x);
        return this->derived();
    }
};


template <typename Iterator>
class identity_proxy : public proxy_base<identity_proxy<Iterator>, Iterator> {

    template <typename T>
    void on_assignment(T const& x) {
        this->do_output(x);
    }

public:
    explicit identity_proxy(Iterator const& out)
        : proxy_base<Iterator>(out)
    { }
};

template <typename Iterator, typename Predicate>
class filter_proxy : public proxy_base<filter_proxy<Iterator>, Iterator> {
    Predicate predicate_;

    template <typename T>
    void on_assignment(T const& x) {
        if (predicate_(x))
            this->do_output(x);
    }

public:
    filter_proxy(Iterator const& out, Predicate const& predicate)
        : proxy_base<Iterator>(out), predicate_(predicate)
    { }
};

template <typename Iterator, typename UnaryFunction>
class transform_proxy : public proxy_base<transform_proxy<Iterator>,Iterator>{
    UnaryFunction f_;

    template <typename T>
    void on_assignment(T const& x) {
        this->do_output(f_(x));
    }

public:
    transform_proxy(Iterator const& out, UnaryFunction const& f)
        : proxy_base<Iterator>(out), f_(f)
    { }
};

template <typename Iterator, typename UnaryFunction>
class indirect_proxy : public proxy_base<indirect_proxy<Iterator>, Iterator> {

    template <typename T>
    void on_assignment(T const& x) {
        this->do_output(*x);
    }

public:
    explicit indirect_proxy(Iterator const& out)
        : proxy_base<Iterator>(out)
    { }
};


template <typename OutputProxy>
class output_iterator {
    OutputProxy proxy_;

public:
    template <typename ...Args>
    explicit output_iterator(Args ...args)
        : proxy_(args...)
    { }

    output_iterator& operator++() {
        ++proxy_.out_;
        return *this;
    }

    output_iterator operator++(int) {
        output_iterator tmp(*this);
        ++proxy_.out_;
        return tmp;
    }

    OutputProxy& operator*() {
        return proxy_;
    }

    OutputProxy const& operator*() const {
        return proxy_;
    }

    template <typename Proxy>
    friend ... operator|(output_iterator const& self, Proxy const& other) {
        return output_proxy<combination>(self.proxy_.combine_with(other));
    }
};

} // end namespace detail

template <typename Iterator> struct is_transform_output_iterator_type;

template <typename UnaryFunction_, typename Iterator>
class transform_output_iterator {
    // Use decay so we can accept function types.
    typedef typename boost::decay<UnaryFunction_>::type UnaryFunction;

    UnaryFunction f_;
    Iterator out_;

    template <typename UnaryFunc, typename Iter>
    class output_proxy {
        UnaryFunc& f_;
        Iter& out_;

    public:
        output_proxy(UnaryFunc& f, Iter& out)
            : f_(f), out_(out)
        { }

        template <typename T>
        output_proxy& operator=(T const& x) {
            *out_ = f_(x);
            return *this;
        }
    };

    // Apply the append metafunction to a given type. This is only required
    // for the eval_if construct below.
    template <typename It, typename F>
    struct apply_append {
        typedef typename It::template append<F>::type type;
    };

    // Metafunction computing the type of a transform_output_iterator applying
    // the G function after applying all the function it currently applies.
    template <typename G>
    struct append {
        typedef transform_output_iterator<UnaryFunction,
            typename mpl::eval_if<is_transform_output_iterator_type<Iterator>,
                apply_append<Iterator, G>,
                mpl::identity<transform_output_iterator<G, Iterator> >
            >::type
        > type;
    };

    // We need to befriend all types of transform_output_iterator because we
    // need to access the append_to_chain method of out_.
    template <typename UF, typename I>
    friend class transform_output_iterator;

    template <typename G>
        typename enable_if<is_transform_output_iterator_type<Iterator>,
    typename append<G>::type>::type append_to_chain(G const& g) const {
        return typename append<G>::type(f_, out_.append_to_chain(g));
    }

    template <typename G>
        typename disable_if<is_transform_output_iterator_type<Iterator>,
    typename append<G>::type>::type append_to_chain(G const& g) const {
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
        return append_to_chain(g);
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

    template <typename G>
    friend typename append<G>::type
                operator|(transform_output_iterator const& self, G const& g) {
        return self.append_to_chain(g);
    }
};

template <typename Iterator>
struct is_transform_output_iterator_type
    : mpl::false_
{ };

template <typename UnaryFunction, typename Iterator>
struct is_transform_output_iterator_type<
                        transform_output_iterator<UnaryFunction, Iterator> >
    : mpl::true_
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
