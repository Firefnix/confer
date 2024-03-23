#include <stddef.h>
#include <confer.h>
#include <assertions.h>

void test_bool_assertions(CFTEST);
void test_pointers_assertions(CFTEST);
void test_int_assertions(CFTEST);
void test_int_equal(CFTEST);
void test_int_compare(CFTEST);
void test_char_assertions(CFTEST);
void test_byte_assertions(CFTEST);
void test_string_assertions(CFTEST);
void test_chain_assertions(CFTEST);

void test_assertions(CFTEST)
{
    cfTest(test_bool_assertions);
    cfTest(test_pointers_assertions);
    cfTest(test_int_assertions);
    cfTest(test_char_assertions);
    cfTest(test_byte_assertions);
    cfTest(test_string_assertions);
    cfTest(test_chain_assertions);
}

void test_pointers_assertions(CFTEST)
{
    assertNull(NULL);
    assertNull(0);
    int a = 3, b = 3;
    int *p = &a;
    int *q = &b;
    int *r = &b;
    assertNotNull(p);
    assertNotNull(&p);

    assertPointerEqual(q, r);
    assertPointerNotEqual(p, q);
    assertPointerNotEqual(p, r);
}

void test_bool_assertions(CFTEST)
{
    assertTrue(true);
    assertTrue(1);
    assertTrue(2);
    assertTrue(-6);

    assertFalse(0);
    assertFalse(false);
}

void test_int_assertions(CFTEST)
{
    cfTest(test_int_equal);
    cfTest(test_int_compare);
}

void test_int_equal(CFTEST)
{
    assertZero(2 - 2);
    assertNonZero(2 - 3);
    assertIntEqual(2 + 4, 6);
    assertIntNotEqual(2 + 4, 5);
}

void test_int_compare(CFTEST)
{
    assertIntGe(3, 2);
    assertIntGe(1, 1);
    assertIntLe(2, 3);
    assertIntLe(1, 1);
    assertIntGreater(3, -2);
    assertIntLess(-2, 3);
}

void test_char_assertions(CFTEST)
{
    char a = 'a', b = 'b';
    assertCharEqual(a, a);
    assertCharEqual(b, b);
    assertCharNotEqual(a, b);
    assertCharNotEqual(b, a);
}

void test_byte_assertions(CFTEST)
{
    unsigned char a = 12, b = 34;
    assertCharEqual(a, a);
    assertCharEqual(b, b);
    assertCharNotEqual(a, b);
    assertCharNotEqual(b, a);
}

void test_string_assertions(CFTEST)
{
    assertStringEqual("blah", "blah");
    assertStringEqual("blah" + 1, "lah");
    assertStringEqual("blah", "blah\0");

    assertStringNotEqual("blah", "bleh");
    assertStringNotEqual("blah", "lah");
    assertStringNotEqual("blah", "bla");
}

bool is_five(int n)
{
    return n == 5;
}

void test_chain_assertions(CFTEST)
{
    assertChain(is_five, 5);
    assertChainNot(is_five, 4);
}
