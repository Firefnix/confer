#ifndef CONFER_H
#define CONFER_H

#include "assertions.h"
#include "build.h"
#include "print.h"

#define cfInit initRootScope

#define CFTEST struct cfScope *self
#define cfTest(f)                   \
    static struct cfScope node_##f; \
    initScope(node_##f);            \
    _cfAddChild(self, &node_##f);   \
    f(&node_##f)

#endif
