#include "../../include/id/idList.h"
#include <stdlib.h>

void idlist_add(IdList *list, Id id) {
    IdAddress new_node = malloc(sizeof(IdElmtList));
    new_node->info = id;
    new_node->nextId = NULL;

    if (list->first == NULL) {
        list->first = new_node;
    } else {
        IdAddress temp = list->first;
        while (temp->nextId != NULL) {
            temp = temp->nextId;
        }
        temp->nextId = new_node;
    }
}

void idlist_delete_by_id(IdList *list, Id target) {
    if (list->first == NULL) return;

    IdAddress prev = NULL;
    IdAddress curr = list->first;

    while (curr != NULL) {
        if (curr->info == target) {
            if (prev == NULL) {
                list->first = curr->nextId;
            } else {
                prev->nextId = curr->nextId;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->nextId;
    }
}