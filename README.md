# Creating a Python iterator in a C extension module

This repo demonstrates creating a Python iterator in a C extension module.

This is the same as creating a generic type except that here the type has
to implement the equivalent of the Python `__next__()` and `__iter__()` dunder
methods.

We implement them as C functions that we assign to the `.tp_iter` and `.tp_iternext`
fields of the `PyTypeObject` defining our class.

## What the iterator does

What the iterator does is not important but for fun I had it return successive
prime numbers and had it keep a cache of previous primes to speed up the
finding the next prime number.

The cache of previous primes also has the benefit that it makes the default
destructor inadequate and forces us to write a non-trivial destructor which
we assign to the `.tp_dealloc` field of our `PyTypeObject`.


