// license here

#ifndef BOOST_PROPERTY_MAP_CHAINED_PROPERTY_MAP_HPP
#define BOOST_PROPERTY_MAP_CHAINED_PROPERTY_MAP_HPP

#include <boost/concept/assert.hpp>
#include <boost/property_map/property_map.hpp> // ReadablePropertyMap concept
#include <boost/property_map/property_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/conditional.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost {

template <typename From, typename To>
class chained_property_map {
    // We need the following because values from the From map
    // are used as keys into the To map.
    BOOST_CONCEPT_ASSERT((ReadablePropertyMapConcept<
                            From,
                            typename property_traits<From>::key_type
                          >));
    BOOST_STATIC_ASSERT(( ::boost::is_convertible<
                            typename property_traits<From>::reference,
                            typename property_traits<To>::key_type
                         >::value));

    // If the To map is not a model of the WritablePropertyMap concept,
    // we can make the reference to it a const reference because we will
    // not modify it.
    typedef typename add_reference<
                typename conditional<
                    !::boost::is_writable_property_map<To>::value,
                    typename add_const<
                        typename remove_reference<To>::type
                    >::type,
                    typename remove_reference<To>::type
                >::type
            >::type StoredTo;

    From const& from_;
    StoredTo to_;

public:
    typedef typename property_traits<From>::key_type key_type;
    typedef typename property_traits<To>::value_type value_type;
    typedef typename property_traits<To>::reference reference;
    typedef typename property_traits<To>::category category;

    chained_property_map(From const& from_map, StoredTo to_map)
        : from_(from_map), to_(to_map) {}

    reference operator[](key_type const& key) {
        // Since we can't make this method disappear completely from
        // resolution when the map does not model the LvaluePropertyMap
        // concept, an error will be triggered if it is instantiated and
        // it should not.
        BOOST_STATIC_ASSERT( ::boost::is_lvalue_property_map<
                                chained_property_map
                            >::value);
        return to_[get(from_, key)];
    }

private:
    // Friend get and put.
    template <typename F, typename T> friend
    typename enable_if<is_readable_property_map<chained_property_map<F, T> >,
    typename property_traits<chained_property_map<F, T> >::reference>::type
    get(chained_property_map<F, T> const&,
        typename property_traits<chained_property_map<F, T> >::key_type const&);

    template <typename F, typename T> friend
    typename enable_if<is_writable_property_map<chained_property_map<F, T> >,
    void>::type
    put(chained_property_map<F, T>&,
        typename property_traits<chained_property_map<F, T> >::key_type const&,
        typename property_traits<chained_property_map<F, T> >::value_type const&);
};

template <typename From, typename To>
typename enable_if<is_readable_property_map<chained_property_map<From, To> >,
typename property_traits<chained_property_map<From, To> >::reference>::type
get(chained_property_map<From, To> const& map,
    typename property_traits<chained_property_map<From, To> >::key_type const& key) {
    return get(map.to_, get(map.from_, key));
}

template <typename From, typename To>
typename enable_if<is_writable_property_map<chained_property_map<From, To> >,
void>::type
put(chained_property_map<From, To>& map,
    typename property_traits<chained_property_map<From, To> >::key_type const& key,
    typename property_traits<chained_property_map<From, To> >::value_type const& value) {
    put(map.to_, get(map.from_, key), value);
}

template <typename From, typename To>
chained_property_map<From, To>
make_chained_property_map(From const& from_map, To& to_map)
{ return chained_property_map<From, To>(from_map, to_map); }

template <typename From, typename To>
chained_property_map<From, To>
make_chained_property_map(From const& from_map, To const& to_map)
{ return chained_property_map<From, To>(from_map, to_map); }

} // end namespace boost

#endif // !BOOST_PROPERTY_MAP_CHAINED_PROPERTY_MAP_HPP
