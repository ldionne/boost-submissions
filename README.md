# boost-submissions
A repository containing my proposals for boost that are under review or not
yet proposed.

Everything has been built and tested under the following configuration:

    $ c++ -v
    Apple clang version 4.1 (tags/Apple/clang-421.11.65) (based on LLVM 3.1svn)
    Target: x86_64-apple-darwin11.4.2
    Thread model: posix

## Proposals

### boost.iterator
#### boost.iterator.accessor\_iterator
The `accessor_iterator` is an iterator adaptor that returns a reference to a
member of its pointee when dereferenced. I see it as another step in the
direction of easier iterator composition, which has helped me writting less
code in the past.

Open questions regarding the final design:

- Should the member accessed by the iterator be determined at compile time,
  or should it be dynamic?

        struct Foo {
            int n;
            int get_n() const
            { return n; }
        };
        typedef std::vector<Foo>::iterator FooIterator;
        std::vector<Foo> vec(10);

        // Example of usage with dynamic pointer to member:
        typedef boost::accessor_iterator<FooIterator, int Foo::*> FooNIterator;
        FooNIterator it(vec.begin(), &Foo::n);

        // Example of usage with compile time pointer to member:
        typedef boost::accessor_iterator<FooIterator, int Foo::*, &Foo::n>
                                                                FooNIterator;
        FooNIterator it(vec.begin());


- How should pointer to member functions be handled? With the current
  naive implementation, trying to use the `accessor_iterator` with a
  pointer to a member function causes a compilation error.

  I have thought of three possible avenues:

    - Do not support pointer to member functions.

    - Return a wrapper like `mem_fn`.

            typedef ... GetNIterator;
            GetNIterator it(vec.begin());
            boost::function<int (Foo const*)> get_n = *it;
            get_n(&*vec.begin()); // equivalent to vec.begin()->get_n()

    - Return a wrapper with its first argument (`this`) already bound, so the
      member function is associated with the object it was obtained from.

            typedef ... GetNIterator;
            GetNIterator it(vec.begin());
            boost::function<int ()> get_n = *it;
            get_n(); // equivalent to vec.begin()->get_n()

- Should the containing object's type be inferred from the adapted iterator's
  `value_type`, or should it be passed explicitly?

        // With inferred containing object:
        typedef boost::accessor_iterator<Iterator, int, &Foo::n> It;

        // With explicit containing object:
        typedef boost::accessor_iterator<Iterator, int Foo::*, &Foo::n> It;


#### boost.iterator.chained\_output\_iterator
The `chained_output_iterator` is an iterator adaptor allowing to apply a
functional transformation to a value before outputing it into another output
iterator. It is much like a `function_output_iterator` that would forward its
result to another output iterator. It allows for cool stuff like creating a
pipeline of operations without having to create temporary storage.

### boost.property\_map
#### boost.property\_map.accessor\_property\_map
The `accessor_property_map` is a property map returning a reference to a
member of the key it is given. It is much like the `accessor_iterator` for
property maps.

#### boost.property\_map.chained\_property\_map
The `chained_property_map` is a tool for property map composition. It takes
two property maps and uses a value in the first one as a key in the second
one to perform its `put`, `get`, and `operator[]` operations.

#### boost.property\_map.property\_traits
I feel like the `boost/property_map/property_map.hpp` header is a little bit
bloated and not quite as modular as it could be. For now, I propose that we
move the code dealing with `property_traits` into the
`boost/property_map/property_traits.hpp` header and that we provide
metafunctions to check for adherence to property map types. By providing
an include in the `boost/property_map/property_map.hpp` header, backward
compatibility is maintained and we simply increase modularity.
