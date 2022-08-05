#ifndef CF_SCOPES_ASSERT_H
#   define CF_SCOPES_ASSERT_H 1
#include <stdbool.h>
#include "build.h"

#define assertIsNull assertNull
#define assertIsNotNull assertNotNull
#define _ASSERT_ARGS struct cfScope *scope, unsigned int line, const char *file
#define ASSERT_ARGS self, __LINE__, __FILE__

int _assertTrue(bool b, _ASSERT_ARGS);
int _assertFalse(bool b, _ASSERT_ARGS);

int _assertNull(void *p, _ASSERT_ARGS);
int _assertNotNull(void *p, _ASSERT_ARGS);
int _assertPointerEqual(const void *p1, const void *p2, _ASSERT_ARGS);
int _assertPointerNotEqual(const void *p1, const void *p2, _ASSERT_ARGS);

int _assertZero(int x, _ASSERT_ARGS);
int _assertNonZero(int x, _ASSERT_ARGS);
int _assertIntEqual(int x, int y, _ASSERT_ARGS);
int _assertIntNotEqual(int x, int y, _ASSERT_ARGS);

int _assertIntGe(int x, int y, _ASSERT_ARGS);
int _assertIntLe(int x, int y, _ASSERT_ARGS);
int _assertIntGreater(int x, int y, _ASSERT_ARGS);
int _assertIntLess(int x, int y, _ASSERT_ARGS);
int _assertIntNonNegative(int x, _ASSERT_ARGS);
int _assertIntNonPositive(int x, _ASSERT_ARGS);
int _assertIntPositive(int x, _ASSERT_ARGS);
int _assertIntNegative(int x, _ASSERT_ARGS);

int _assertCharEqual(char c1, char c2, _ASSERT_ARGS);
int _assertCharNotEqual(char c1, char c2, _ASSERT_ARGS);

int _assertByteEqual(unsigned char b1, unsigned char b2, _ASSERT_ARGS);
int _assertByteNotEqual(unsigned char b1, unsigned char b2, _ASSERT_ARGS);

int _assertStringEqual(const char *s1, const char *s2, _ASSERT_ARGS);
int _assertStringNotEqual(const char *s1, const char *s2, _ASSERT_ARGS);


#define assertTrue(b) _assertTrue(b, ASSERT_ARGS)
#define assertFalse(b) _assertFalse(b, ASSERT_ARGS)

#define assertNull(p) _assertNull(p, ASSERT_ARGS)
#define assertNotNull(p) _assertNotNull(p, ASSERT_ARGS)
#define assertPointerEqual(p1, p2) _assertPointerEqual(p1, p2, ASSERT_ARGS)
#define assertPointerNotEqual(p1, p2) _assertPointerNotEqual(p1, p2, ASSERT_ARGS)

#define assertZero(x) _assertZero(x, ASSERT_ARGS)
#define assertNonZero(x) _assertNonZero(x, ASSERT_ARGS)
#define assertIntEqual(x, y) _assertIntEqual(x, y, ASSERT_ARGS)
#define assertIntNotEqual(x, y) _assertIntNotEqual(x, y, ASSERT_ARGS)

#define assertIntGe(x, y) _assertIntGe(x, y, ASSERT_ARGS)
#define assertIntLe(x, y) _assertIntLe(x, y, ASSERT_ARGS)
#define assertIntGreater(x, y) _assertIntGreater(x, y, ASSERT_ARGS)
#define assertIntLess(x, y) _assertIntLess(x, y, ASSERT_ARGS)
#define assertIntNonNegative(x) _assertIntNonNegative(x, ASSERT_ARGS)
#define assertIntNonPositive(x) _assertIntNonPositive(x, ASSERT_ARGS)
#define assertIntPositive(x) _assertIntPositive(x, ASSERT_ARGS)
#define assertIntNegative(x) _assertIntNegative(x, ASSERT_ARGS)

#define assertCharEqual(x, y) _assertCharEqual(x, y, ASSERT_ARGS)
#define assertCharNotEqual(x, y) _assertCharNotEqual(x, y, ASSERT_ARGS)

#define assertStringEqual(s1, s2) _assertStringEqual(s1, s2, ASSERT_ARGS)
#define assertStringNotEqual(p1, p2) _assertStringNotEqual(p1, p2, ASSERT_ARGS)


#define assertCustom(f, ...) assertTrue(f(__VA_ARGS__))
#define assertCustomNot(f, ...) assertFalse(f(__VA_ARGS__))

#endif
