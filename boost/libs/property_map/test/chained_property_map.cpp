// license here

#include <boost/property_map/chained_property_map.hpp>
#include <boost/property_map/function_property_map.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/property_traits.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <cassert>


namespace {

template <int n>
struct Add {
    typedef int result_type;
    result_type operator()(int i) const { return i + n; }
};

// Types used in the tests below.
typedef boost::typed_identity_property_map<int> Identity;
typedef boost::function_property_map<Add<2>, int> Plus2;
typedef boost::vector_property_map<int> Vector;


void should_support_same_concept_as_second_property_map() {
    BOOST_CONCEPT_ASSERT((boost::ReadablePropertyMapConcept<
                            boost::chained_property_map<Identity, Plus2>,
                            boost::property_traits<Identity>::key_type
                        >));
    BOOST_CONCEPT_ASSERT((boost::Mutable_LvaluePropertyMapConcept<
                            boost::chained_property_map<Identity, Vector>,
                            boost::property_traits<Identity>::key_type
                        >));
    BOOST_CONCEPT_ASSERT((boost::LvaluePropertyMapConcept<
                            boost::chained_property_map<Identity, Vector const>,
                            boost::property_traits<Identity>::key_type
                        >));
}

void should_map_to_correct_values() {
    typedef boost::chained_property_map<Identity, Plus2> Pmap;
    Pmap map = Pmap(Identity(), Plus2());
    assert(get(map, 1) == 3);
    assert(get(map, 2) == 4);
}

void should_put_to_second_map() {
    typedef boost::chained_property_map<Identity, Vector> Pmap;
    Vector vec_map;
    Pmap map = Pmap(Identity(), vec_map);
    for (int i = 0; i < 10; ++i)
        put(map, i, i);

    for (int i = 0; i < 10; ++i)
        assert(get(map, i) == i);

    for (int i = 0; i < 10; ++i)
        assert(get(map, i) == map[i]);
}

void should_allow_chains() {
    typedef boost::chained_property_map<Plus2, Vector> Plus2Vector;
    typedef boost::chained_property_map<Identity, Plus2Vector> Pmap;
    Vector vec;
    Plus2Vector p2vec(Plus2(), vec);
    Pmap map(Identity(), p2vec);
    map[1] = 10;
    assert(get(map, 1) == 10);

    put(map, 2, 20);
    assert(get(map, 2) == 20);
}

} // end anonymous namespace


int main(int, char const*[]) {
    should_support_same_concept_as_second_property_map();
    should_map_to_correct_values();
    should_put_to_second_map();
    should_allow_chains();
    return 0;
}
