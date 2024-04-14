#include <stdio.h>
#include <string.h>
#include "print.h"
#include "children.h"

#ifndef __unix__
/* Windows' CMD does not support UTF-8 and ASCII special sequences */
#define CF_USE_ASCII
#endif

#ifdef CF_USE_ASCII
static const char *hbar = "-", *vbar = "|", *branch = "|", *angle = "`",
                  *checkMark = "OK", *notCheckMark = "X",
                  *passed = "passed", *failed = "failed", *bold = "",
                  *end = "";
#else
static const char *hbar = "─", *vbar = "│", *branch = "├", *angle = "└",
                  *checkMark = "\033[32m✓\033[0m",
                  *notCheckMark = "\033[31m✗\033[0m", // ✘
                      *passed = "\033[32m✓\033[0m",
                  *failed = "\033[31m✗\033[0m", *bold = "\033[1m",
                  *end = "\033[0m";
#endif
static const char *nbsp = " ";
unsigned int TAB_SIZE = 4;

static void printPadding(const char *head, const char *body);
static char *getPaddingHead(struct cfScope *s, const unsigned int depth);
static char *getPaddingBody(const unsigned int depth);
static char *getScopeHead(struct cfScope *s);
static void _cfPrintScopeAssertions(struct cfScope *s);

void _cfPrintAssertionFail(const char *file, const char *assertionType, const unsigned int line, const struct cfScope *scope)
{
    printf("%s%s:%u%s: in function %s: %s", bold, file, line, end, scope->name, assertionType + 1);
}

int _cfReturnCode(struct cfScope *root)
{
    if (countAssertionsFailed(root) == 0)
    {
        return 0;
    }
    return 1;
}

int _cfPrintResults(struct cfScope *root)
{
    puts("\nTotal:");
    unsigned int passed = countAssertionsPassed(root);
    unsigned int failed = countAssertionsFailed(root);
    if (passed)
        printf("%s %u passed main blocks\n%s %u passed assertions\n", checkMark,
               countChildrenPassed(root), checkMark, passed);
    if (failed)
        printf("%s %u failed main blocks\n%s %u failed assertions\n", notCheckMark,
               countChildrenFailed(root), notCheckMark, failed);
    if (!failed && !passed)
        fprintf(stderr, "WARNING: scope '%s' has no failed or passed blocks\n",
                root->name);
    return 0;
}

void printTree(struct cfScope *s)
{
    if (s->parent != NULL)
        fprintf(stderr, "WARNING: node '%s' at %p has a parent '%s' at %p\n",
                s->name, (void *)s, s->parent->name, (void *)s->parent);
    printScope(s, 0);
}

void printScope(struct cfScope *s, const int depth)
{
    if (s == NULL)
        return;
    for (unsigned int i = depth; i; --i)
    {
        printPadding(getPaddingHead(s, i), getPaddingBody(i));
        putc(' ', stdout);
    }
    printf("%s %s", getScopeHead(s), s->name);
    _cfPrintScopeAssertions(s);
    for (int i = 0; i < CF_MAX_CHILDREN; ++i)
    {
        printScope(s->children[i], depth + 1);
    }
}

static void _cfPrintScopeAssertions(struct cfScope *s)
{
    if (!s->assertions.passed && !s->assertions.failed)
    {
        putc('\n', stdout);
        return;
    }
    fputs(" (", stdout);
    if (s->assertions.passed && !s->assertions.failed)
        printf("%u %s", s->assertions.passed, passed);
    else if (!s->assertions.passed && s->assertions.failed)
        printf("%u %s", s->assertions.failed, failed);
    else
        printf("%u %s and %u %s", s->assertions.passed, passed, s->assertions.failed, failed);
    puts(")");
}

static void printPadding(const char *head, const char *body)
{
    fputs(head, stdout);
    for (unsigned int i = 0; i < TAB_SIZE - 2; ++i)
    {
        fputs(body, stdout);
    }
}

static char *getScopeHead(struct cfScope *s)
{
    static char head[13];
    if (s->assertions.failed || hasChildrenWithErrors(s))
        strcpy(head, notCheckMark);
    else
        strcpy(head, checkMark);
    return head;
}

static char *getPaddingHead(struct cfScope *s, const unsigned int depth)
{
    static char head[4];
    if (isLastChild(getParent(s, depth - 1)))
    {
        if (depth == 1)
            strcpy(head, angle);
        else
            strcpy(head, nbsp);
    }
    else
    {
        if (depth == 1)
            strcpy(head, branch);
        else
            strcpy(head, vbar);
    }
    return head;
}

static char *getPaddingBody(const unsigned int depth)
{
    static char body[4];
    if (depth == 1)
        strcpy(body, hbar);
    else
        strcpy(body, nbsp);
    return body;
}
