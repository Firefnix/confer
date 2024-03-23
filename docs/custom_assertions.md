---
title: Custom Assertions
---

Custom Assertions
=================

In addition to the few dozens of built-in assertions that come with it, Confer offers a way to create your own assertions based on any boolean function.

The easiest way to create custom assertion functions is as for now to use `assertChain`, as explained below.

{% warning Possible changes %}
The recommended way to create custom assertions may change in the future.
{% end %}

### First example

Let's say that we use rational numbers with the type `rational`. We want to create a custom assertion to check if a rational is equal to zero and if two rationals are equal.

```c
#define assertIsZero(x) assertChain(is_zero, x)

void test_zero(CFTEST) {
    rational z = zero();
    assertIsZero(z); // passes
    assertTrue(is_zero(z)); // an equivalent
}
```

### Multiple arguments

The magic of `assertChain` is that you can use it with as many arguments as you want! Just add their names between the parentheses after your custom assertion's name, and report them after your boolean function.

```c
#define assertRationalEqual(x, y) assertChain(are_equal_rational, x, y)

void test_are_equal() {
    rational a = my_rational();
    assertRationalEqual(a, a); // passes
}
```
