---
title: Example
---

Example
========

The best way to learn Confer in a few minutes is to use it. In this example we will create a small library named `calc` to add and integers.

## Creating our project

The first step it to create a new project named `calc`. Enter `cf create calc --lib` in a terminal. This will:
* Create a `calc` directory
* Create `src`, `test`, `obj` and `bin` directories inside it
* Create a pre-filled C file at `test/test.c`
* Create a pre-filled `Makefile`

## Adding code

Now it's time to write our first piece of code: a function to add two integers. This function named `plus` will be part of the `operations` object.

An object is really a pair of files : a C language file (here, `operations.c`) and a C header file (here, `operations.h`). You can see an object as a library submodule.

To add a new `operations` object, enter `cf add operations` in a terminal. This will create two pre-filled files:
A `src/operations.h` header file:
```c
#ifndef OPERATIONS_H
#define OPERATIONS_H



#endif
```
And a `src/operations.c` code file:
```c
#include <operations.h>
```

Now we can write code. Open and edit these two files in a code editor so that they contain this:

In `src/operations.h`:
```c
#ifndef OPERATIONS_H
#define OPERATIONS_H

int plus(int a, int b);

#endif
```

In `src/operations.c`:
```c
#include <src/operations.h>

int plus(int a, int b) { return a + b; }
```

## Adding tests

We are already reaching the final part of this tutorial: writing tests. Open `test/test.c` in a code editor.

For now, `test/test.c` contains only:
```c
#include<confer.h>

int main(int argc, char *argv[]) {
    return 0;
}
```

First, replace `main`'s arguments by `void` (we will not use arguments for now), and add calls to two Confer functions, `cfInit` and `cfPrintCallTree`.

The first one is to initialize the Confer library, and the second one to print the results of our future tests.

We also have to import our `operations` object through its header file.

Now our `test/test.c` file looks like this:
```c
#include <confer.h>
#include <operations.h>

int main(void) {
    cfInit();
    cfPrintCallTree();
    return 0;
}
```

Now we can write our test: from `main`, we will call the subtest `test_operations`, that will itself call `test_plus`. Calling a subtest from a function is easy, we just have to use the `cfTest` function. Once this is done, our `test/test.c` file will look like this:
```c
#include <confer.h>
#include <operations.h>

void test_operations(CFTEST);
void test_plus(CFTEST);

int main(void) {
    cfInit();
    cfTest(test_operations);
    cfPrintCallTree();
    return 0;
}

void test_operations(CFTEST) { cfTest(test_plus); }

void test_plus(CFTEST) {}
```

{% success 👍 Good practice %}
For a better readability, place the functions' signature on top of the file.
{% end %}

Finally, we can test our `plus` function using Confer's built-in assertions:
```c
void test_plus(CFTEST) {
    assertIntEqual(plus(1, 2), 3);
    assertIntNotEqual(plus(1, -2), 0);
}
```

## Running the tests

Now that the library and the tests are written, all we have to do is to run the tests.

In a terminal, simply enter `cf test`, and voilà!
```
$ cf test
⚙ src/operations.c -> obj/operations.o
✓ main
└── ✓ test_operations
    └── ✓ test_plus (2 ✓)
```

You can always add new tests and subtests by creating new `CFTEST` functions in your `test.c` file.

{% success 🔭 Multiple test files %}
You can also create new test files and include them to your main `test.c` file.
{% end %}
