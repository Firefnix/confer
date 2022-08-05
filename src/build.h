#ifndef NODES_BUILD_H
#   define NODES_BUILD_H 1

#define CF_MAX_CHILDREN 256
#define MAX_NAME_SIZE 256

struct cfTestsCount {
    unsigned int passed, failed;
};

struct cfScope {
	char name[MAX_NAME_SIZE];
	struct cfScope *parent;
	struct cfScope *children[CF_MAX_CHILDREN]; /* NULL-terminated list of pointers */
	struct cfTestsCount assertions;
};

#define initScope(s) _initScope(&s, #s)

#define initRootScope() \
    struct cfScope node_main; \
    initScope(node_main); \
    static struct cfScope *self; \
    self = &node_main

void _initScope(struct cfScope *s, const char *name);
void _cfAddChild(struct cfScope *parent, struct cfScope *child);

#endif
