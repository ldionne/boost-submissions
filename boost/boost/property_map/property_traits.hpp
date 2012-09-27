// license here

#ifndef BOOST_PROPERTY_MAP_PROPERTY_TRAITS_HPP
#define BOOST_PROPERTY_MAP_PROPERTY_TRAITS_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/if.hpp>
#include <boost/pending/cstddef.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost {

/*
 * Property traits class
 */
BOOST_MPL_HAS_XXX_TRAIT_DEF(key_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(reference)
BOOST_MPL_HAS_XXX_TRAIT_DEF(category)

template<class PA>
struct is_property_map
    : boost::mpl::and_<
        has_key_type<PA>,
        has_value_type<PA>,
        has_reference<PA>,
        has_category<PA>
    >
{ };

template <typename PA>
struct default_property_traits {
    typedef typename PA::key_type key_type;
    typedef typename PA::value_type value_type;
    typedef typename PA::reference reference;
    typedef typename PA::category category;
};

struct null_property_traits { };

template <typename PA>
struct property_traits
    : boost::mpl::if_<is_property_map<PA>,
        default_property_traits<PA>,
        null_property_traits
    >::type
{ };


/*
 * Property traits category tags
 */
namespace detail {
    enum ePropertyMapID {
        READABLE_PA,
        WRITABLE_PA,
        READ_WRITE_PA,
        LVALUE_PA,
        OP_BRACKET_PA,
        RAND_ACCESS_ITER_PA,
        LAST_PA
    };
} // end namespace detail

struct readable_property_map_tag
{ enum { id = detail::READABLE_PA }; };

struct writable_property_map_tag
{ enum { id = detail::WRITABLE_PA }; };

struct read_write_property_map_tag
    : readable_property_map_tag, writable_property_map_tag
{ enum { id = detail::READ_WRITE_PA }; };

struct lvalue_property_map_tag : read_write_property_map_tag
{ enum { id = detail::LVALUE_PA }; };


/*
 * Metafunctions to probe types for supported property map concepts
 */
template <typename M>
struct is_readable_property_map
    : mpl::and_<
        is_property_map<M>,
        is_convertible<
            typename property_traits<M>::category,
            readable_property_map_tag
        >
    >
{ };

template <typename M>
struct is_writable_property_map
    : mpl::and_<
        is_property_map<M>,
        is_convertible<
            typename property_traits<M>::category,
            writable_property_map_tag
        >
    >
{ };

template <typename M>
struct is_read_write_property_map
    : mpl::and_<
        is_property_map<M>,
        is_convertible<
            typename property_traits<M>::category,
            read_write_property_map_tag
        >
    >
{ };

template <typename M>
struct is_lvalue_property_map
    : mpl::and_<
        is_property_map<M>,
        is_convertible<
            typename property_traits<M>::category,
            lvalue_property_map_tag
        >
    >
{ };

template <typename M>
struct is_mutable_lvalue_property_map
    : mpl::and_<
        is_lvalue_property_map<M>,
        is_same<
            typename property_traits<M>::value_type&,
            typename property_traits<M>::reference
        >
    >
{ };

template <typename M>
struct is_non_mutable_lvalue_property_map
    : mpl::and_<
        is_lvalue_property_map<M>,
        is_same<
            typename property_traits<M>::value_type const&,
            typename property_traits<M>::reference
        >
    >
{ };


/*
 * Property traits specialization for pointers
 */
#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
// The user will just have to create their own specializations for
// other pointers types if the compiler does not have partial
// specializations. Sorry!
#define BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(TYPE)                          \
    template <>                                                             \
    struct property_traits<TYPE*> {                                         \
        typedef TYPE value_type;                                            \
        typedef value_type& reference;                                      \
        typedef std::ptrdiff_t key_type;                                    \
        typedef lvalue_property_map_tag   category;                         \
    };                                                                      \
    template <>                                                             \
    struct property_traits<const TYPE*> {                                   \
        typedef TYPE value_type;                                            \
        typedef const value_type& reference;                                \
        typedef std::ptrdiff_t key_type;                                    \
        typedef lvalue_property_map_tag   category;                         \
    }

BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(long);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(unsigned long);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(int);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(unsigned int);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(short);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(unsigned short);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(char);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(unsigned char);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(signed char);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(bool);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(float);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(double);
BOOST_SPECIALIZE_PROPERTY_TRAITS_PTR(long double);

// This may need to be turned off for some older compilers that don't have
// wchar_t intrinsically.
#ifndef BOOST_NO_INTRINSIC_WCHAR_T
template <>
struct property_traits<wchar_t*> {
    typedef wchar_t value_type;
    typedef value_type& reference;
    typedef std::ptrdiff_t key_type;
    typedef lvalue_property_map_tag   category;
};

template <>
struct property_traits<const wchar_t*> {
    typedef wchar_t value_type;
    typedef const value_type& reference;
    typedef std::ptrdiff_t key_type;
    typedef lvalue_property_map_tag   category;
};
#endif // !BOOST_NO_INTRINSIC_WCHAR_T

#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION is not defined

template <class T>
struct property_traits<T*> {
    typedef T value_type;
    typedef value_type& reference;
    typedef std::ptrdiff_t key_type;
    typedef lvalue_property_map_tag category;
};

template <class T>
struct property_traits<const T*> {
    typedef T value_type;
    typedef const value_type& reference;
    typedef std::ptrdiff_t key_type;
    typedef lvalue_property_map_tag category;
};

#endif // !BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
// MSVC doesn't have Koenig lookup, so the user has to
// do boost::get() anyways, and the using clause
// doesn't really work for MSVC.

} // end namespace boost
#endif

// These need to go in global namespace because Koenig
// lookup does not apply to T*.

// V must be convertible to T
template <class T, class V>
inline void put(T* pa, std::ptrdiff_t k, const V& val) { pa[k] = val;  }

template <class T>
inline const T& get(const T* pa, std::ptrdiff_t k) { return pa[k]; }

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
namespace boost {
    using ::put;
    using ::get;
#endif

} // end namespace boost

#endif // !BOOST_PROPERTY_MAP_PROPERTY_TRAITS_HPP
