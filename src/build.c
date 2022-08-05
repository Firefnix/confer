#include "build.h"
#include <string.h>
#include <stdio.h>

void _initScope(struct cfScope *s, const char *name)
{
	static const unsigned int NODE_PREFIX_SIZE = 5;
	strcpy(s->name, name + NODE_PREFIX_SIZE);
	for (unsigned int i = 0; i < CF_MAX_CHILDREN; ++i) {
		s->children[i] = NULL;
	}
	s->assertions = (struct cfTestsCount){0, 0};
}

void _cfAddChild(struct cfScope *parent, struct cfScope *child)
{
	struct cfScope *p;
	unsigned int i = 0;
	if (child->parent != NULL) {
		fprintf(stderr, "WARNING: overriding the parent of node '%s', from \
node '%s' to node '%s'.\nThis may cause a segmentation fault.", child->name,
child->parent->name, parent->name);
	}

	child->parent = parent;
	do {
		p = parent->children[i];
		++i;
	} while (p != NULL && i < CF_MAX_CHILDREN);
	if (i < CF_MAX_CHILDREN)
		parent->children[i-1] = child;
}
