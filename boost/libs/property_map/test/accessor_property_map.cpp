// license here

#include <boost/property_map/accessor_property_map.hpp>
#include <cassert>
#include <string>


namespace {

struct Person {
    Person(std::string const& name_, unsigned short age_)
        : name(name_), age(age_) { }
    std::string name;
    unsigned short age;
};

typedef boost::accessor_property_map<std::string Person::*, &Person::name> NameOf;
typedef boost::accessor_property_map<unsigned short Person::*, &Person::age> AgeOf;


void should_return_right_attribute() {
    Person john("john", 30);
    assert(get(NameOf(), john) == "john");
    assert(get(AgeOf(), john) == 30);
}

void get_with_mutable_object() {
    Person john("john", 30);
    get(NameOf(), john) = "modified";
    assert(john.name == "modified");
}

void put_should_change_attribute_value() {
    Person john("john", 30);
    put(NameOf(), john, "modified");
    assert(john.name == "modified");
}

void subscript_operator_with_mutable_object() {
    Person john("john", 30);
    std::string& name = NameOf()[john];
    assert(name == john.name);
    name = "modified";
    assert(name == john.name);
}

} // end anonymous namespace


int main(int, char const*[]) {
    should_return_right_attribute();
    get_with_mutable_object();
    put_should_change_attribute_value();
    subscript_operator_with_mutable_object();
    return 0;
}
