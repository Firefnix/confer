#include <stdio.h>
#include "children.h"

struct cfScope *getParent(struct cfScope *s, unsigned int depth)
{
	struct cfScope *p = s;
	for (; p && p->parent && depth; p = p->parent, --depth);
	return p;
}

int getChildIndex(struct cfScope *s)
{
	if (s->parent == NULL)
		return -1;
	for (unsigned int i = 0; i < CF_MAX_CHILDREN - 1; ++i) {
		if (s->parent->children[i] == s)
			return i;
	}
	return -1;
}

bool isLastChild(struct cfScope *s)
{
	int index = getChildIndex(s);
	if (index == -1) {
		fprintf(stderr, "ERROR: Cannot find parent index of node '%s' at %p.\nWARNING: This will probably cause a segmentation fault.\n", s->name, (void *)s);
		return false;
	}
	return s->parent->children[index + 1] == NULL;
}

bool hasChildrenWithErrors(struct cfScope *scope)
{
    return (countChildrenFailed(scope) != 0);
}

unsigned int countChildren(struct cfScope *scope)
{
    unsigned int c = 0;
    for (unsigned int i = 0; i < CF_MAX_CHILDREN; ++i) {
        if (scope->children[i] != NULL)
            ++c;
    }
    return c;
}

unsigned int countChildrenPassed(struct cfScope *scope)
{
    unsigned int c = 0;
    for (unsigned int i = 0; i < CF_MAX_CHILDREN; ++i) {
        if (scope->children[i] != NULL && scope->children[i]->assertions.failed == 0)
            ++c;
    }
    return c;
}

unsigned int countChildrenFailed(struct cfScope *scope)
{
    unsigned int c = 0;
    for (unsigned int i = 0; i < CF_MAX_CHILDREN; ++i) {
        if (scope->children[i] != NULL && scope->children[i]->assertions.failed)
            ++c;
    }
    return c;
}

unsigned int countAssertionsPassed(struct cfScope *scope)
{
	unsigned int sum = 0;
	for (unsigned int i = 0; i < CF_MAX_CHILDREN; ++i) {
        if (scope->children[i] != NULL) {
            sum += scope->children[i]->assertions.passed;
			if (countChildren(scope->children[i]))
				sum += countAssertionsPassed(scope->children[i]);
		}
    }
	return sum;
}

unsigned int countAssertionsFailed(struct cfScope *scope)
{
	unsigned int sum = 0;
	for (unsigned int i = 0; i < CF_MAX_CHILDREN; ++i) {
        if (scope->children[i] != NULL) {
            sum += scope->children[i]->assertions.failed;
			if (countChildren(scope->children[i]))
				sum += countAssertionsFailed(scope->children[i]);
		}
    }
	return sum;
}
