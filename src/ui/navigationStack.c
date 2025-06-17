#include "../../include/ui/navigationStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void navigation_stack_create(NavigationStack *S) {
    S->top = NULL;
}

bool navigation_stack_is_empty(NavigationStack S) {
    return S.top == NULL;
}

void navigation_stack_push(NavigationStack *S, const char *page_name) {
    if (!navigation_stack_is_empty(*S) &&
        strcmp(S->top->page_name, page_name) == 0) {
        return; // Jangan push kalau page sama
    }

    NavigationAddress new_node = malloc(sizeof(NavigationElmt));
    strcpy(new_node->page_name, page_name);
    new_node->next = S->top;
    S->top = new_node;
}

void navigation_stack_pop(NavigationStack *S) {
    if (!navigation_stack_is_empty(*S)) {
        NavigationAddress temp = S->top;
        S->top = S->top->next;
        free(temp);
    }
}

char *navigation_stack_top(NavigationStack S) {
    return navigation_stack_is_empty(S) ? NULL : S.top->page_name;
}

void navigation_stack_clear(NavigationStack *S) {
    while (!navigation_stack_is_empty(*S)) {
        navigation_stack_pop(S);
    }
}

void save_navigation_stack(NavigationStack S, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;

    // Simpan stack ke file (dari top ke bottom)
    NavigationAddress current = S.top;
    while (current != NULL) {
        fprintf(f, "%s\n", current->page_name);
        current = current->next;
    }

    fclose(f);
}

Id navigation_stack_get_last_post_id(NavigationStack *stack) {
    NavigationAddress current = stack->top;
    while (current != NULL) {
        if (strncmp(current->page_name, "post:", 5) == 0) {
        return atoi(current->page_name + 5);
    }
    current = current->next;
    }
  return -1; // Tidak ditemukan
}

Id navigation_stack_get_last_comment_id(NavigationStack *stack) {
    NavigationAddress current = stack->top;
    while (current != NULL) {
        if (strncmp(current->page_name, "comment:", 8) == 0) {
        return atoi(current->page_name + 8);
    }
    current = current->next;
    }
    return -1; // Tidak ditemukan
}

void load_navigation_stack(NavigationStack *S, const char *filename) {
    navigation_stack_clear(S);
    FILE *f = fopen(filename, "r");
    if (!f) return;

    // Sementara pakai array untuk membalik urutan
    char pages[100][MAX_PAGE_NAME];
    int count = 0;

    while (fgets(pages[count], MAX_PAGE_NAME, f)) {
        pages[count][strcspn(pages[count], "\n")] = 0; // Remove newline
        count++;
    }

    // Masukkan ke stack dari bawah ke atas
    int i = count - 1;
    for (i = count - 1; i >= 0; i--) {
        navigation_stack_push(S, pages[i]);
    }

    fclose(f);
}
