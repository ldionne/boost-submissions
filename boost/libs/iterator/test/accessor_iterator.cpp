// license here

#include <algorithm>
#include <boost/iterator/accessor_iterator.hpp>
#include <cassert>
#include <iterator>
#include <string>
#include <vector>


namespace {

struct Person {
    Person(std::string const& name_, unsigned short age_)
        : name(name_), age(age_) { }
    std::string name;
    unsigned short age;
};

typedef std::vector<Person> Persons;
typedef Persons::iterator PersonIterator;
typedef boost::accessor_iterator<PersonIterator, std::string> NameIterator;
typedef boost::accessor_iterator<PersonIterator, unsigned short> AgeIterator;

void should_return_right_attribute() {
    Persons persons;
    Person john("john", 30);
    persons.push_back(john);
    NameIterator name_of_john(persons.begin(), &Person::name);
    AgeIterator age_of_john(persons.begin(), &Person::age);
    assert(*name_of_john == "john");
    assert(*age_of_john == 30);
}

void should_return_modifiable_reference() {
    Persons persons;
    Person john("john", 30);
    persons.push_back(john);
    NameIterator name_of_john(persons.begin(), &Person::name);
    *name_of_john = "modified";
    assert(*name_of_john == "modified");
    assert(persons.begin()->name == "modified");
}

void should_work_in_standard_algorithm() {
    Persons persons;
    persons.push_back(Person("a", 1));
    persons.push_back(Person("b", 2));
    persons.push_back(Person("c", 3));
    persons.push_back(Person("d", 4));

    std::vector<unsigned short> ages;
    std::copy(AgeIterator(persons.begin(), &Person::age),
              AgeIterator(persons.end(), &Person::age),
              std::back_inserter(ages));

    std::vector<unsigned short> ages_expected;
    ages_expected.push_back(1);
    ages_expected.push_back(2);
    ages_expected.push_back(3);
    ages_expected.push_back(4);

    assert(ages == ages_expected);
}

void test_with_make_function() {
    Persons persons;
    persons.push_back(Person("john", 30));
    AgeIterator age = boost::make_accessor_iterator(persons.begin(),
                                                    &Person::age);
    assert(*age == 30);
}

} // end anonymous namespace


int main(int, char const*[]) {
    should_return_right_attribute();
    should_return_modifiable_reference();
    should_work_in_standard_algorithm();
    test_with_make_function();
    return 0;
}
