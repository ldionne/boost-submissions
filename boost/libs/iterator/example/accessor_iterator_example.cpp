// license here

#include <boost/iterator/accessor_iterator.hpp>
#include <sstream>
#include <string>
#include <vector>


namespace {

struct Person {
    int age;
    std::string first_name;
    std::string last_name;
};

template <typename Ostream>
Ostream& operator<<(Ostream& os, Person const& p) {
    os << p.first_name << ' ' << p.last_name << ", " << p.age;
    return os;
}

template <typename Istream>
Istream& operator>>(Istream& is, Person& p) {
    char comma, space;
    is >> p.first_name >> space >> p.last_name >> comma >> space >> p.age;
    return is;
}

std::string const dataset =
"Loris Pardini, 32\n"
"Sharyn Breault, 40\n"
"Joella Race, 24\n"
"Mavis Almquist, 30\n"
"Tracey Canova, 56\n"
"Elvis Beaupre, 20\n"
"Carmelita Romney, 12\n"
"Hester Dehne, 38\n"
"Tod Bravo, 41\n"
"Wiley Nigh, 57\n";


} // end anonymous namespace

#include <iostream>
int main(int, char const*[]) {
    std::istringstream is(dataset);
    std::istream_iterator<Person> first(is), last;
    std::vector<Person> persons(first, last);
    std::cout << dataset;

    std::copy(persons.begin(), persons.end(), std::ostream_iterator<Person>(std::cout));


    return 0;
}
