#ifndef NODES_PRINT_H
#define NODES_PRINT_H 1

#include "build.h"

unsigned int TAB_SIZE;

#define cfPrintCallTree() printTree(&node_main);
#define cfPrintResults() _cfPrintResults(self)
#define cfReturnCode() _cfReturnCode(&node_main)

void printTree(struct cfScope *root);
void printScope(struct cfScope *s, const int depth);
int _cfPrintResults(struct cfScope *root);
void _cfPrintAssertionFail(const char *file, const char *assertionType, const unsigned int line, const struct cfScope *scope);
int _cfReturnCode(struct cfScope *root);

#endif
