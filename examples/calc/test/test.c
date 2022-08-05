#include <confer.h>
#include <plus.h>
#include <minus.h>

void test_plus(CFTEST);
void test_minus(CFTEST);

int main(void) {
    cfInit();
    cfTest(test_plus);
    cfTest(test_minus);
    cfPrintCallTree();
    return 0;
}

void test_plus(CFTEST) {
    assertIntEqual(plus(1, 2), 3);
    assertIntNotEqual(plus(1, -2), 0);
}

void test_minus(CFTEST) {
    assertIntEqual(minus(4, 3), 1);
    assertIntGreater(minus(4, 3), 0);
}
