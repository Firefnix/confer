Confer
======

Confer is a C and C++ unit testing library written in C. It is currently only available on Linux, and limited to GCC.

## Features

Confer comes with a number of tools and feaures out of the box.
  -  20+ built-in assertions for most common types
  -  Custom assertions
  -  Fast build (the example compiles and runs in ~100 ms)
  -  Extra-light library written in C, no dynamic memory allocation (hence no memory leaks)
  -  A handy tool to manage projects and run tests (not compulsory)

## Preview

To start using Confer, simply <place installation instructions here>, and include it in your test file:

```c
#include <confer.h>
```

Then, after calling the initialization function, you will be able to write your first tests:

```c
void main(void) {
  cfInit(); // nothing more needed
  cfTest(test_my_function); // call subtests
  cfPrintCallTree(); // show the results
}

void test_my_function(CFTEST) { // make it a test
  assertIntEqual(my_function(0), 1);
  assertIntGreater(my_function(2), 7);
}
```

## What next?

To know more about the library, we also recommend you visiting Confer's [documentation](https://github.com/Firefnix/confer/tree/main/docs). For a fast overview of Confer's capacity, you can also check the [example](https://github.com/Firefnix/confer/tree/main/examples/calc).

## Where can I get help?

Feel free to open issues on the [Github Repo](https://github.com/Firefnix/confer), especially if
you did not find an answer to a question in our documentation. You can also reach out directly to
the maintainer via email at firefnix@gmail.com.
