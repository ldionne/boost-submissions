// Copyright Louis Dionne 2012. Use, modification and distribution is subject
// to the Boost Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/property_map/property_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/static_assert.hpp>

struct dummy { };
typedef boost::readable_property_map_archetype<dummy, dummy> readable_property_map_archetype;
typedef boost::writable_property_map_archetype<dummy, dummy> writable_property_map_archetype;
typedef boost::read_write_property_map_archetype<dummy, dummy> read_write_property_map_archetype;
typedef boost::lvalue_property_map_archetype<dummy, dummy> lvalue_property_map_archetype;
typedef boost::mutable_lvalue_property_map_archetype<dummy, dummy> mutable_lvalue_property_map_archetype;

#define ASSERT_PMAP(pmap) \
    BOOST_STATIC_ASSERT(::boost::is_property_map<pmap>::value)
ASSERT_PMAP(readable_property_map_archetype);
ASSERT_PMAP(writable_property_map_archetype);
ASSERT_PMAP(read_write_property_map_archetype);
ASSERT_PMAP(lvalue_property_map_archetype);
ASSERT_PMAP(mutable_lvalue_property_map_archetype);
#undef ASSERT_PMAP


#define ASSERT_READABLE_PMAP(pmap) \
    BOOST_STATIC_ASSERT(::boost::is_readable_property_map<pmap>::value)
ASSERT_READABLE_PMAP(readable_property_map_archetype);
BOOST_STATIC_ASSERT(!::boost::is_readable_property_map<
                        writable_property_map_archetype
                    >::value);
ASSERT_READABLE_PMAP(read_write_property_map_archetype);
ASSERT_READABLE_PMAP(lvalue_property_map_archetype);
ASSERT_READABLE_PMAP(mutable_lvalue_property_map_archetype);
#undef ASSERT_READABLE_PMAP


#define ASSERT_WRITABLE_PMAP(pmap) \
    BOOST_STATIC_ASSERT(::boost::is_writable_property_map<pmap>::value)
BOOST_STATIC_ASSERT(!::boost::is_writable_property_map<
                        readable_property_map_archetype
                    >::value);
ASSERT_WRITABLE_PMAP(writable_property_map_archetype);
ASSERT_WRITABLE_PMAP(read_write_property_map_archetype);
ASSERT_WRITABLE_PMAP(lvalue_property_map_archetype);
ASSERT_WRITABLE_PMAP(mutable_lvalue_property_map_archetype);
#undef ASSERT_WRITABLE_PMAP


#define ASSERT_READ_WRITE_PMAP(pmap) \
    BOOST_STATIC_ASSERT(::boost::is_read_write_property_map<pmap>::value)
#define ASSERT_NOT_READ_WRITE_PMAP(pmap) \
    BOOST_STATIC_ASSERT(!::boost::is_read_write_property_map<pmap>::value)
ASSERT_NOT_READ_WRITE_PMAP(readable_property_map_archetype);
ASSERT_NOT_READ_WRITE_PMAP(writable_property_map_archetype);
ASSERT_READ_WRITE_PMAP(read_write_property_map_archetype);
ASSERT_READ_WRITE_PMAP(lvalue_property_map_archetype);
ASSERT_READ_WRITE_PMAP(mutable_lvalue_property_map_archetype);
#undef ASSERT_NOT_READ_WRITE_PMAP
#undef ASSERT_READ_WRITE_PMAP


#define ASSERT_LVALUE_PMAP(pmap) \
    BOOST_STATIC_ASSERT(::boost::is_lvalue_property_map<pmap>::value)
#define ASSERT_NOT_LVALUE_PMAP(pmap) \
    BOOST_STATIC_ASSERT(!::boost::is_lvalue_property_map<pmap>::value)
ASSERT_NOT_LVALUE_PMAP(readable_property_map_archetype);
ASSERT_NOT_LVALUE_PMAP(writable_property_map_archetype);
ASSERT_NOT_LVALUE_PMAP(read_write_property_map_archetype);
ASSERT_LVALUE_PMAP(lvalue_property_map_archetype);
ASSERT_LVALUE_PMAP(mutable_lvalue_property_map_archetype);
#undef ASSERT_NOT_LVALUE_PMAP
#undef ASSERT_LVALUE_PMAP


#define ASSERT_MUTABLE_LVALUE_PMAP(pmap) \
    BOOST_STATIC_ASSERT(::boost::is_mutable_lvalue_property_map<pmap>::value)
#define ASSERT_NOT_MUTABLE_LVALUE_PMAP(pmap) \
    BOOST_STATIC_ASSERT(!::boost::is_mutable_lvalue_property_map<pmap>::value)
ASSERT_NOT_MUTABLE_LVALUE_PMAP(readable_property_map_archetype);
ASSERT_NOT_MUTABLE_LVALUE_PMAP(writable_property_map_archetype);
ASSERT_NOT_MUTABLE_LVALUE_PMAP(read_write_property_map_archetype);
ASSERT_NOT_MUTABLE_LVALUE_PMAP(lvalue_property_map_archetype);
ASSERT_MUTABLE_LVALUE_PMAP(mutable_lvalue_property_map_archetype);
#undef ASSERT_NOT_MUTABLE_LVALUE_PMAP
#undef ASSERT_MUTABLE_LVALUE_PMAP


int main(int, char const*[]) {
    return 0;
}
