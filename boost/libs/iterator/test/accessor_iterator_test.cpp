// (C) Copyright 2013 Louis Dionne
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <boost/concept/assert.hpp>
#include <boost/iterator/accessor_iterator.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <cassert>
#include <iterator>
#include <string>
#include <vector>


namespace accessor_iterator_test {

struct Person {
    Person(std::string const& name_, unsigned short age_)
        : name(name_), age(age_) { }
    std::string name;
    unsigned short age;
};

BOOST_CONCEPT_ASSERT((boost_concepts::WritableIterator<
                        boost::accessor_iterator<
                            std::vector<Person>::iterator,
                            std::string Person::*,
                            &Person::name
                        >,
                        std::string
                    >));

BOOST_CONCEPT_ASSERT((boost_concepts::ReadableIterator<
                        boost::accessor_iterator<
                            std::vector<Person>::const_iterator,
                            std::string const Person::*,
                            &Person::name
                        >
                    >));

BOOST_CONCEPT_ASSERT((boost_concepts::LvalueIterator<
                        boost::accessor_iterator<
                            std::vector<Person>::iterator,
                            std::string Person::*,
                            &Person::name
                        >
                    >));

BOOST_CONCEPT_ASSERT((boost_concepts::RandomAccessTraversal<
                        boost::accessor_iterator<
                            std::vector<Person>::iterator,
                            std::string Person::*,
                            &Person::name
                        >
                    >));

typedef std::vector<Person> Persons;
typedef Persons::iterator PersonIterator;
typedef boost::accessor_iterator<PersonIterator, std::string Person::*, &Person::name> NameIterator;
typedef boost::accessor_iterator<PersonIterator, unsigned short Person::*, &Person::age> AgeIterator;

void should_return_right_attribute() {
    Persons persons;
    Person john("john", 30);
    persons.push_back(john);
    NameIterator name_of_john(persons.begin());
    AgeIterator age_of_john(persons.begin());
    assert(*name_of_john == "john");
    assert(*age_of_john == 30);
}

void should_return_modifiable_reference() {
    Persons persons;
    Person john("john", 30);
    persons.push_back(john);
    NameIterator name_of_john(persons.begin());
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
    std::copy(AgeIterator(persons.begin()),
              AgeIterator(persons.end()),
              std::back_inserter(ages));

    std::vector<unsigned short> ages_expected;
    ages_expected.push_back(1);
    ages_expected.push_back(2);
    ages_expected.push_back(3);
    ages_expected.push_back(4);

    assert(ages == ages_expected);
}

} // end accessor_iterator_test namespace


int main() {
    using namespace accessor_iterator_test;
    should_return_right_attribute();
    should_return_modifiable_reference();
    should_work_in_standard_algorithm();
    return 0;
}
