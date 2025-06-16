#ifndef NAVIGATION_STACK_H
#define NAVIGATION_STACK_H

#include <stdbool.h>

#define MAX_PAGE_NAME 100

typedef struct NavigationNode *NavigationAddress;
typedef struct NavigationNode {
    char page_name[MAX_PAGE_NAME];
    NavigationAddress next;
} NavigationElmt;

typedef struct {
    NavigationAddress top;
} NavigationStack;

// Stack operations
void navigation_stack_create(NavigationStack *S);
bool navigation_stack_is_empty(NavigationStack S);
void navigation_stack_push(NavigationStack *S, const char *page_name);
void navigation_stack_pop(NavigationStack *S);
char *navigation_stack_top(NavigationStack S);
void navigation_stack_clear(NavigationStack *S);

// File persistence
void save_navigation_stack(NavigationStack S, const char *filename);
void load_navigation_stack(NavigationStack *S, const char *filename);

#endif
