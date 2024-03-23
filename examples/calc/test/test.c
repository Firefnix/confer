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

void test_plus(CFTEST) {
    assertIntEqual(plus(1, 2), 3);
    assertIntNotEqual(plus(1, -2), 0);
}
