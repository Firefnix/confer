#include <stdio.h>
#include <confer.h>
#include <test_assertions.c>

void test_return_value(CFTEST);

int main(void) {
    cfInit();
    cfTest(test_assertions);
    cfTest(test_return_value);
    cfPrintCallTree();
    cfPrintResults();
    return 0;
}

void test_return_value(CFTEST) {
    if (assertTrue(true)) {
        assertNull(NULL); // ok, great
    }
    /* Works well, but commented because it outputs a failed assertion
    (as expected)

    if (!assertFalse(true)) {
        assertNull(NULL);
    } */
}
