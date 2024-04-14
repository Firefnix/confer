#ifndef CF_NODES_CHILDREN_H
#define CF_NODES_CHILDREN_H 1

#define CF_MAX_DEPTH 999

#include <stdbool.h>
#include "build.h"
bool isLastChild(struct cfScope *s);
int getChildIndex(struct cfScope *s);
struct cfScope *getParent(struct cfScope *s, unsigned int depth);

bool hasChildrenWithErrors(struct cfScope *scope);
unsigned int countChildren(struct cfScope *scope);
unsigned int countChildrenPassed(struct cfScope *scope);
unsigned int countChildrenFailed(struct cfScope *scope);

unsigned int countAssertionsPassed(struct cfScope *scope);
unsigned int countAssertionsFailed(struct cfScope *scope);

#endif
