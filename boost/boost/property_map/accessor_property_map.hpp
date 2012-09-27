// license here

#ifndef BOOST_PROPERTY_MAP_ACCESSOR_PROPERTY_MAP_HPP
#define BOOST_PROPERTY_MAP_ACCESSOR_PROPERTY_MAP_HPP

#include <boost/property_map/property_traits.hpp>


namespace boost {

namespace detail {
    /**
     * Metafunction extracting the tag and the member type of a
     * pointer to member type.
     */
    template <typename T> struct parse_memptr;
    template <typename Tag, typename Member>
    struct parse_memptr<Member Tag::*> {
        typedef Tag tag_type;
        typedef Member member_type;
        typedef Member Tag::*member_ptr_type;
    };
} // end namespace detail

/**
 * A property map whose key is an object and whose value
 * is a member of the object.
 */
template <typename MemberPtr, MemberPtr memptr>
struct accessor_property_map {
    typedef typename detail::parse_memptr<MemberPtr>::tag_type key_type;
    typedef typename detail::parse_memptr<MemberPtr>::member_type value_type;
    typedef value_type& reference;
    typedef lvalue_property_map_tag category;

    value_type const& operator[](key_type const& key) const
    { return key.*memptr; }

    reference operator[](key_type& key) const
    { return key.*memptr; }
};

template <typename MemberPtr, MemberPtr memptr>
typename property_traits<accessor_property_map<MemberPtr, memptr> >::reference
get(accessor_property_map<MemberPtr, memptr> const&,
    typename property_traits<accessor_property_map<MemberPtr, memptr> >::key_type& key) {
    return key.*memptr;
}

template <typename MemberPtr, MemberPtr memptr>
typename property_traits<accessor_property_map<MemberPtr, memptr> >::value_type const&
get(accessor_property_map<MemberPtr, memptr> const&,
    typename property_traits<accessor_property_map<MemberPtr, memptr> >::key_type const& key) {
    return key.*memptr;
}

template <typename MemberPtr, MemberPtr memptr>
void
put(accessor_property_map<MemberPtr, memptr> const&,
    typename property_traits<accessor_property_map<MemberPtr, memptr> >::key_type& key,
    typename property_traits<accessor_property_map<MemberPtr, memptr> >::value_type const& value) {
    key.*memptr = value;
}

} // end namespace boost

#endif // !BOOST_PROPERTY_MAP_ACCESSOR_PROPERTY_MAP_HPP
