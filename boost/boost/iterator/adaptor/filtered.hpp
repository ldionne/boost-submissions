// (C) Copyright 2012 Louis Dionne
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ITERATOR_ADAPTOR_FILTERED_HPP
#define BOOST_ITERATOR_ADAPTOR_FILTERED_HPP

namespace boost {
namespace iterator {
namespace adaptor {

template <typename Derived>
struct adaptor_base {
    template <typename UnaryFunction, typename Iterator, typename G>
    friend ....
    operator|(transform_output_iterator<UnaryFunction, Iterator> const& out,
              adaptor_base const& self) {
        return out.replace_output_proxy(self.get_output_proxy());
    }
};

namespace detail {
struct filtered_type : adaptor_base<filtered_type> {
    template <typename Predicate>
    ... operator()(Predicate const& pred) const {
        return ;
    }
};
} // end namespace detail

static detail::filtered_type const filtered;

} // end namespace adaptor
} // end namespace iterator
} // end namespace boost

#endif // !BOOST_ITERATOR_ADAPTOR_FILTERED_HPP
