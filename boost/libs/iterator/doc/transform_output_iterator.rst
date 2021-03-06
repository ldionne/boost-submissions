.. Distributed under the Boost
.. Software License, Version 1.0. (See accompanying
.. file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

++++++++++++++++++++++++++
 Transform Output Iterator
++++++++++++++++++++++++++

:Author: Louis Dionne
:Contact: ldionne.2@gmail.com
:copyright: Copyright Louis Dionne 2013

:abstract:
    The transform output iterator creates an output iterator applying a
    functional transformation to a value before outputting it in another
    iterator. It can be seen like the output iterator counterpart of the
    transform iterator.

.. contents:: Table of Contents


``transform_output_iterator`` synopsis
......................................

::

    template <typename UnaryFunction, typename Iterator>
    class transform_output_iterator {
        UnaryFunction f_; // exposition only
        Iterator out_; // exposition only

        // exposition only
        template <typename UnaryFunction_, typename Iterator_>
        class output_proxy;

    public:
        typedef void value_type;
        typedef void reference;
        typedef void pointer;
        typedef void difference_type;
        typedef incrementable_traversal_tag iterator_category;

        template <typename G>
        /* see below */ and_then(G const& g = G()) const;

        template <typename G>
        friend /* see below */ operator|(transform_output_iterator const&, G const&);

        explicit transform_output_iterator(UnaryFunction const& f, Iterator const& iterator);

        explicit transform_output_iterator(Iterator const& iterator);

        transform_output_iterator& operator++();
        transform_output_iterator operator++(int);

        output_proxy<UnaryFunction, Iterator> operator*();
        output_proxy<UnaryFunction const, Iterator const> operator*() const;
    };


``transform_output_iterator`` requirements
..........................................
The type ``UnaryFunction`` must be CopyConstructible and Callable with any
type that is assigned to the result of dereferencing the
``transform_output_iterator``. ``Iterator`` must model the WritableIterator,
Incrementable and CopyConstructible concepts.

Callable Concept
----------------
A type models the Callable concept when it supports the call operator. For
given objects ``f``, ``x`` of type ``F``, respectively ``X``, the following
construct should be valid for ``F`` to model the Callable with ``X`` concept:

=========  =================  ===========
Construct  Description        Return Type
---------  -----------------  -----------
f(x)       Call f on x        unspecified
=========  =================  ===========

Incrementable Concept (shamelessly taken from ``function_input_iterator``'s documentation)
------------------------------------------------------------------------------------------
A type models the Incrementable Concept when it supports the pre- and post-
increment operators. For a given object ``i`` with type ``I``, the following
constructs should be valid:

=========  =================  ===========
Construct  Description        Return Type
---------  -----------------  -----------
i++        Post-increment i.  I
++i        Pre-increment i.   I&
=========  =================  ===========


``transform_output_iterator`` models
....................................
``transform_output_iterator`` is a model of the WritableIterator, the
IncrementableIterator and the OutputIterator concepts.


``transform_output_iterator`` operations
........................................


``explicit transform_output_iterator(UnaryFunction const& f, Iterator const& iterator);``

:Effects:
    Constructs an instance of ``transform_output_iterator`` with ``f_``
    constructed from ``f`` and ``out_`` constructed from ``iterator``.


``explicit transform_output_iterator(Iterator const& iterator);``

:Effects:
    Constructs an instance of ``transform_output_iterator`` with ``f_``
    default constructed and ``out_`` constructed from ``iterator``.


``transform_output_iterator& operator++();``

:Effects: ``++out_``.
:Returns: ``*this``.


``transform_output_iterator& operator++(int);``

:Effects:

::

    transform_output_iterator tmp(*this);
    ++*this;
    return tmp;


``output_proxy<UnaryFunction, Iterator> operator*();``

:Returns:
    An output proxy applying ``f_`` to the value assigned to it before
    forwarding the result to ``out_``.


``output_proxy<UnaryFunction const, Iterator const> operator*() const;``

:Returns:
    An output proxy applying ``f_`` to the value assigned to it before
    forwarding the result to ``out_``. Calling ``f_`` and dereferencing
    ``out_`` must be operations marked with ``const``.

::

    template <typename G>
    transform_output_iterator<unspecified_type1, unspecified_type2>
    and_then(G const& g = G()) const;

:Returns:
    A ``transform_output_iterator`` with unspecified template parameters
    composing ``g`` and ``f_``. The functional transformation applied by the
    returned iterator is equivalent to ``g(f_(x))``, which means that ``g``
    is applied to the result of calling ``f_``.


::

    template <typename G>
    friend transform_output_iterator<unspecified_type1, unspecified_type2>
    operator|(transform_output_iterator const& self, G const& g);

:Effects:
    Equivalent to ``self.and_then(g)``.

::

    template <typename UnaryFunction, typename Iterator>
    transform_output_iterator<UnaryFunction, Iterator>
    make_transform_output_iterator(UnaryFunction const& f, Iterator const& iterator);

:Returns:
    An instance of ``transform_output_iterator<UnaryFunction, Iterator>`` with
    ``f_`` initialized to ``f`` and ``out_`` initialized to ``iterator``.


::

    template <typename UnaryFunction, typename Iterator>
    transform_output_iterator<UnaryFunction, Iterator>
    make_transform_output_iterator(Iterator const& iterator);

:Returns:
    An instance of ``transform_output_iterator<UnaryFunction, Iterator>`` with
    ``f_`` default constructed and ``out_`` initialized to ``iterator``.


::

    template <typename Iterator>
    struct is_transform_output_iterator_type;

MPL-compatible metafunction to determine whether a type is a
``transform_output_iterator``. ``is_transform_output_iterator_type`` inherits
``boost::mpl::true_`` if ``Iterator`` is a ``transform_output_iterator`` and
``boost::mpl::false_`` otherwise.


::

    template <typename Iterator>
    bool is_transform_output_iterator(Iterator const&);

:Returns: ``::boost::is_transform_output_iterator_type<Iterator>::value``.


Example 1
.........

This is an example of using the ``transform_output_iterator`` to multiply each
element of an array before printing it.

::

    #include <algorithm>
    #include <boost/iterator/transform_output_iterator.hpp>
    #include <iostream>
    #include <iterator>


    template <int n>
    struct MultiplyBy {
        int operator()(int x) const {
            return n * x;
        }
    };

    typedef boost::transform_output_iterator<MultiplyBy<2>,
                std::ostream_iterator<int> > DoublingIterator;

    int main() {
        int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        int const N = sizeof(array)/sizeof(array[0]);

        DoublingIterator out(std::ostream_iterator<int>(std::cout, " "));

        std::cout << "double each element in the array before printing it\n";
        std::copy(array, array + N, out);
        std::cout << std::endl;
    }

The output is::

    double each element in the array before printing it
    0 2 4 6 8 10 12 14 16 18


Example 2
.........

This is an example of composing ``transform_output_iterator``s to create a
pipeline of operations.

::

    #include <boost/iterator/transform_output_iterator.hpp>
    #include <iostream>
    #include <string>


    struct Append {
        std::string s_;
        Append(std::string const& s) : s_(s) { }
        std::string operator()(std::string const& str) const {
            return str + s_;
        }
    };

    int main() {
        std::string result;
        *(boost::make_transform_output_iterator(Append("w"), &result)
                                            |   Append("o")
                                            |   Append("r")
                                            |   Append("l")
                                            |   Append("d"))  = "hello ";
        std::cout << result << std::endl;
    }

The output is::

    hello world


Example 3
.........

This is an example showing similarities between the ``transform_output_iterator``
and the ``transform_iterator``. Depending on the circumstances, using the
``transform_output_iterator`` may sometimes be the best alternative.

::

    #include <algorithm>
    #include <boost/iterator/transform_iterator.hpp>
    #include <boost/iterator/transform_output_iterator.hpp>
    #include <boost/lambda/bind.hpp>
    #include <boost/lambda/lambda.hpp>
    #include <boost/range/begin.hpp>
    #include <boost/range/end.hpp>
    #include <iostream>
    #include <iterator>
    #include <string>


    using namespace boost;

    struct Person {
        std::string name;
        unsigned int age;
    };

    int main() {
        Person dataset[] = {
            {"Louis Dionne", 20},
            {"Sylvester Stallone", 66},
            {"Rick Astley", 46},
            {"John Doe", 30}
        };

        std::ostream_iterator<std::string> out(std::cout, " ");

        // Use the transform_iterator to extract the name of each person.
        std::copy(make_transform_iterator(begin(dataset), lambda::bind(&Person::name, lambda::_1)),
                  make_transform_iterator(end(dataset), lambda::bind(&Person::name, lambda::_1)),
                  out);

        std::cout << '\n';

        // Use the transform_output_iterator to extract the name of each person.
        std::copy(begin(dataset),
                  end(dataset),
                  make_transform_output_iterator(lambda::bind(&Person::name, lambda::_1), out));
    }

The output is::

    Louis Dionne Sylvester Stallone Rick Astley John Doe
    Louis Dionne Sylvester Stallone Rick Astley John Doe

Like you see, in that scenario, using the ``transform_output_iterator`` allows
us to type the functor only once whereas using the ``transform_iterator``
requires typing it twice; once for the begin iterator and once for the end
iterator. Also note that using the ``transform_output_iterator`` would allow
us to chain more functors while the ``transform_iterator`` does not.
