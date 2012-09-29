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
    : public iterator_adaptor<
                accessor_iterator<Iterator, MemberType>,
                Iterator,
                MemberType
    > {
    friend class iterator_core_access;
    typedef typename accessor_iterator::iterator_adaptor_ Adaptor;
    typedef typename iterator_value<Iterator>::type Tag;

    typedef MemberType Tag::* MemberPtr;
    MemberPtr memptr_;

    MemberType& dereference() const
    { return (*this->base_reference()).*memptr_; }

public:
    accessor_iterator() { }

    explicit accessor_iterator(Iterator const& iterator, MemberPtr memptr)
        : Adaptor(iterator), memptr_(memptr) { }
};

template <typename Iterator, typename Tag, typename MemberType>
accessor_iterator<Iterator, MemberType>
make_accessor_iterator(Iterator const& iterator, MemberType Tag::* memptr) {
    return accessor_iterator<Iterator, MemberType>(iterator, memptr);
}

} // end namespace boost

#endif // !BOOST_ITERATOR_ACCESSOR_ITERATOR_HPP
