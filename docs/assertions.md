---
title: Assertions
---

Assertions
==========

Confer comes with more than 20 built-in assertions. You can call them from any `CFTEST` or from your `main` function. Here is an example with `assertIntEqual`:

```c
void test_plus(CFTEST) {
    assertIntEqual(2 + 3, 5);
}
```

{% info Calling subtests %}
To run tests in `test_plus`, call it with `cfTest(test_plus);` from any `CFTEST` or from your `main` function.
{% end %}

Assertions return `true` if they pass, `false` if they fail. This return value can be used in conditions to make more tests:
```c
if (assertNotNull(my_car_pointer)) {
    assertTrue(is_red(*my_car_pointer));
}
```
This prevents errors like null dereferences or semgentation faults.

## Constants

```c
#define ASSERTION_PASSED true
#define ASSERTION_FAILED false
```

## Built-in assertions

### Booleans

```c
int assertTrue(bool b);
```

```c
int assertFalse(bool b);
```

Test that `b` is true (or false).

### Pointers

```c
int assertNull(void *p);
```

```c
int assertNotNull(void *p);
```

Test that `p` is (or is not) `NULL`.

```c
int assertPointerEqual(const void *p1, const void *p2);
```

```c
int assertPointerNotEqual(const void *p1, const void *p2);
```

Test that `p1` and `p2` are (or are not) equal.

### Integers

```c
int assertZero(int x);
```

```c
int assertNonZero(int x);
```

Test that `x` is (or is not) equal to `0`.

```c
int assertIntEqual(int x, int y);
```

```c
int assertIntNotEqual(int x, int y);
```

Test that `x` and `y` are (or are not) equal.

```c
int assertIntGe(int x, int y);
```

Test that `x` is greater than or equal to `y` (passes if `x >= y`).

```c
int assertIntLe(int x, int y);
```

Test that `x` is less than or equal to `y` (passes if `x <= y`).

```c
int assertIntGreater(int x, int y);
```

Test that `x` is greater than `y` (passes if `x > y`).

```c
int assertIntLess(int x, int y);
```

Test that `x` is less than `y` (passes if `x < y`).

```c
int assertIntNonNegative(int x);
```

Test that `x` is non-negative; that is, greater than or equal to `0` (passes if `x >= 0`).

```c
int assertIntNonPositive(int x);
```

Test that `x` is non-positive; that is, less than or equal to `0` (passes if `x <= 0`).

```c
int assertIntPositive(int x);
```

Test that `x` is positive; that is, greater than `0` (passes if `x > 0`).

```c
int assertIntNegative(int x);
```

Test that `x` is negative; that is, less than `0` (passes if `x < 0`).

### Characters & bytes

```c
int assertCharEqual(char c1, char c2);
```

```c
int assertCharNotEqual(char c1, char c2);
```

Test that `c1` and `c2` are (or are not) equal.

```c
int assertByteEqual(unsigned char b1, unsigned char b2);
```

```c
int assertByteNotEqual(unsigned char b1, unsigned char b2);
```

Test that `b1` and `b2` are (or are not) equal.

### Strings

```c
int assertStringEqual(const char *s1, const char *s2);
```

Test that `s1` and `s2` are equal (passes if `strcmp(s1, s2) == 0`).

```c
int assertStringNotEqual(const char *s1, const char *s2);
```

Test that `s1` and `s2` are not equal (passes if `strcmp(s1, s2) != 0`).
