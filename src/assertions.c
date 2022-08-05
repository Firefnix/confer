#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "assertions.h"
#include "print.h"

#define ASSERTION_PASSED true
#define ASSERTION_FAILED false

#define CF_ASSERTIONS_CONTEXT_ARGS struct cfScope *scope, unsigned int line, const char *file
#define CF_ASSERTIONS_CONTEXT scope, line, file

static char *boolToString(bool b);

#define _assert(condition, CF_ASSERTIONS_CONTEXT, format, ...) \
	if (condition) { \
		++scope->assertions.passed; \
		return ASSERTION_PASSED; \
	} else { \
		++scope->assertions.failed; \
		_cfPrintAssertionFail(file, __func__, line, scope); \
        printf(format __VA_OPT__(,) __VA_ARGS__); \
        putc('\n', stdout); \
        return ASSERTION_FAILED; \
	}

int _assertNull(void *p, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(p == NULL, CF_ASSERTIONS_CONTEXT, "(%p): is not null", p);
}
int _assertNotNull(void *p, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(p != NULL, CF_ASSERTIONS_CONTEXT, "(NULL): is null");
}
int _assertPointerEqual(const void *p1, const void *p2, CF_ASSERTIONS_CONTEXT_ARGS) {
    _assert(p1 == p2, CF_ASSERTIONS_CONTEXT, "(%p, %p): not the same pointer", p1, p2);
}
int _assertPointerNotEqual(const void *p1, const void *p2, CF_ASSERTIONS_CONTEXT_ARGS) {
    _assert(p1 != p2, CF_ASSERTIONS_CONTEXT, "(%p, %p): the same pointer", p1, p2);
}

int _assertTrue(bool b, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(b == true, CF_ASSERTIONS_CONTEXT, "(%s): is not true", boolToString(b));
}
int _assertFalse(bool b, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(b == false, CF_ASSERTIONS_CONTEXT, "(%s): is not false", boolToString(b));
}

int _assertZero(int x, CF_ASSERTIONS_CONTEXT_ARGS) {
    _assert(x == 0, CF_ASSERTIONS_CONTEXT, "(%d): is not zero", x);
}
int _assertNonZero(int x, CF_ASSERTIONS_CONTEXT_ARGS) {
    _assert(x, CF_ASSERTIONS_CONTEXT, "(0): is zero");
}

int _assertIntEqual(int x, int y, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(x == y, CF_ASSERTIONS_CONTEXT, "(%d, %d): are not equal", x, y);
}
int _assertIntNotEqual(int x, int y, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(x != y, CF_ASSERTIONS_CONTEXT, "(%d, %d): are equal", x, y);
}

int _assertIntGe(int x, int y, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(x >= y, CF_ASSERTIONS_CONTEXT, "(%d, %d): not greater or equal", x, y);
}
int _assertIntLe(int x, int y, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(x <= y, CF_ASSERTIONS_CONTEXT, "(%d, %d): not less or equal equal", x, y);
}
int _assertIntGreater(int x, int y, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(x > y, CF_ASSERTIONS_CONTEXT, "(%d, %d): not greater", x, y);
}
int _assertIntLess(int x, int y, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(x < y, CF_ASSERTIONS_CONTEXT, "(%d, %d): not less", x, y);
}
int _assertIntNonNegative(int x, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(x >= 0, CF_ASSERTIONS_CONTEXT, "(%d): negative", x);
}
int _assertIntNonPositive(int x, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(x <= 0, CF_ASSERTIONS_CONTEXT, "(%d): positive", x);
}
int _assertIntPositive(int x, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(x > 0, CF_ASSERTIONS_CONTEXT, "(%d): negative or zero", x);
}
int _assertIntNegative(int x, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(x < 0, CF_ASSERTIONS_CONTEXT, "(%d): positive or zero", x);
}

int _assertCharEqual(char c1, char c2, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(c1 == c2, CF_ASSERTIONS_CONTEXT, "(%c, %c): not the same character", c1, c2);
}
int _assertCharNotEqual(char c1, char c2, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(c1 != c2, CF_ASSERTIONS_CONTEXT, "(%c, %c): are the same character", c1, c2);
}

int _assertByteEqual(unsigned char b1, unsigned char b2, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(b1 == b2, CF_ASSERTIONS_CONTEXT, "(%d, %d): not the same byte", b1, b2);
}
int _assertByteNotEqual(unsigned char b1, unsigned char b2, CF_ASSERTIONS_CONTEXT_ARGS) {
	_assert(b1 != b2, CF_ASSERTIONS_CONTEXT, "(%d, %d): are the same byte", b1, b2);
}

int _assertStringEqual(const char *s1, const char *s2, CF_ASSERTIONS_CONTEXT_ARGS) {
    _assert(strcmp(s1, s2) == 0, CF_ASSERTIONS_CONTEXT, "(%s, %s): are not equal", s1, s2);
}
int _assertStringNotEqual(const char *s1, const char *s2, CF_ASSERTIONS_CONTEXT_ARGS) {
    _assert(strcmp(s1, s2) != 0, CF_ASSERTIONS_CONTEXT, "(\"%s\", \"%s\"): are equal", s1, s2);
}

static char *boolToString(bool b)
{
    static char s[6];
    if (b == false)
        strcpy(s, "false");
    else if (b == true)
        strcpy(s, "true");
    else
        sprintf(s, "%d", b);
    return s;
}
