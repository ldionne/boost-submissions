// license here

#ifndef BOOST_ITERATOR_ACCESSOR_ITERATOR_HPP
#define BOOST_ITERATOR_ACCESSOR_ITERATOR_HPP

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_traits.hpp>


namespace boost {

/**
 * Adaptor returning a reference to a member of its adapted
 * iterator's pointee when dereferenced.
 */
template <typename Iterator, typename MemberType>
class accessor_iterator
    : public iterator_adaptor<accessor_iterator<Iterator, MemberType>,
                              Iterator, MemberType> {
    friend class iterator_core_access;
    typedef iterator_adaptor<accessor_iterator, Iterator, MemberType> Adaptor;

    typedef typename iterator_value<Iterator>::type Tag;
    typedef MemberType Tag::* MemberPtr;

    MemberPtr memptr_;

    MemberType& dereference() const
    { return (*this->base_reference()).*memptr_; }

public:
    explicit accessor_iterator(Iterator const& iterator, MemberPtr p)
        : Adaptor(iterator), memptr_(p) { }

    accessor_iterator(accessor_iterator const& other)
        : Adaptor(other), memptr_(other.memptr_) { }
};

template <typename Iterator, typename Tag, typename MemberType>
accessor_iterator<Iterator, MemberType>
make_accessor_iterator(Iterator const& iterator, MemberType Tag::* p)
{ return accessor_iterator<Iterator, MemberType>(iterator, p); }

} // end namespace boost

#endif // !BOOST_ITERATOR_ACCESSOR_ITERATOR_HPP
