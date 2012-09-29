// license here

#include <algorithm>
#include <boost/concept/assert.hpp>
#include <boost/iterator/accessor_iterator.hpp>
#include <boost/iterator/iterator_concepts.hpp>
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

BOOST_CONCEPT_ASSERT((boost_concepts::WritableIterator<
                        boost::accessor_iterator<
                            std::vector<Person>::iterator,
                            std::string
                        >,
                        std::string
                    >));

BOOST_CONCEPT_ASSERT((boost_concepts::ReadableIterator<
                        boost::accessor_iterator<
                            std::vector<Person>::const_iterator,
                            std::string const
                        >
                    >));

BOOST_CONCEPT_ASSERT((boost_concepts::LvalueIterator<
                        boost::accessor_iterator<
                            std::vector<Person>::iterator,
                            std::string
                        >
                    >));

BOOST_CONCEPT_ASSERT((boost_concepts::RandomAccessTraversal<
                        boost::accessor_iterator<
                            std::vector<Person>::iterator,
                            std::string
                        >
                    >));

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

void should_work_with_make_function() {
    Persons persons;
    persons.push_back(Person("a", 1));
    persons.push_back(Person("b", 2));
    persons.push_back(Person("c", 3));

    std::vector<std::string> names;
    std::copy(boost::make_accessor_iterator(persons.begin(), &Person::name),
              boost::make_accessor_iterator(persons.end(), &Person::name),
              std::back_inserter(names));

    std::vector<std::string> expected;
    expected.push_back("a");
    expected.push_back("b");
    expected.push_back("c");

    assert(names == expected);
}

} // end anonymous namespace


int main(int, char const*[]) {
    should_return_right_attribute();
    should_return_modifiable_reference();
    should_work_in_standard_algorithm();
    should_work_with_make_function();
    return 0;
}
