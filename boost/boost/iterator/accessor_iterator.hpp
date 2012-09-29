// license here

#ifndef BOOST_ITERATOR_ACCESSOR_ITERATOR_HPP
#define BOOST_ITERATOR_ACCESSOR_ITERATOR_HPP

#include <boost/iterator/iterator_adaptor.hpp>


namespace boost {

/**
 * Adaptor returning a reference to a member of its adapted
 * iterator's pointee when dereferenced.
 */
template <typename Iterator, typename MemberPtr, MemberPtr memptr>
class accessor_iterator;

template <typename Iterator, typename Tag,
          typename MemberType, MemberType Tag::* memptr>
class accessor_iterator<Iterator, MemberType Tag::*, memptr>
    : public iterator_adaptor<
                accessor_iterator<Iterator, MemberType Tag::*, memptr>,
                Iterator,
                MemberType
    > {
    friend class iterator_core_access;
    typedef typename accessor_iterator::iterator_adaptor_ Adaptor;

    MemberType& dereference() const
    { return (*this->base_reference()).*memptr; }

public:
    accessor_iterator() { }

    explicit accessor_iterator(Iterator const& iterator)
        : Adaptor(iterator) { }
};

} // end namespace boost

#endif // !BOOST_ITERATOR_ACCESSOR_ITERATOR_HPP
