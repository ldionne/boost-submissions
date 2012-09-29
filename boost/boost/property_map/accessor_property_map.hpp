// license here

#ifndef BOOST_PROPERTY_MAP_ACCESSOR_PROPERTY_MAP_HPP
#define BOOST_PROPERTY_MAP_ACCESSOR_PROPERTY_MAP_HPP

#include <boost/property_map/property_traits.hpp>


namespace boost {

/**
 * A property map whose key is an object and whose value
 * is a member of the object.
 */
template <typename MemberPtr, MemberPtr memptr>
struct accessor_property_map;

template <typename Tag, typename MemberType, MemberType Tag::* memptr>
struct accessor_property_map<MemberType Tag::*, memptr> {
    typedef Tag key_type;
    typedef MemberType value_type;
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
