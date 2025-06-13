#include "../../include/moderateThis/moderateThisList.h"
#include <stdlib.h>
#include <stdio.h>

// Create an empty ModerateList
void moderate_create_list(ModerateList *L) {
    L->first = NULL;
    L->id_max = 0;
}

// Create a new ModerateNode
void moderate_create_node(ModerateAddress *p) {
    *p = (ModerateAddress)malloc(sizeof(ModerateElmtList));
    if (*p != NULL) {
        (*p)->next = NULL;
    }
}

// Initialize a ModerateNode with a ModerateRequest
void moderate_isi_node(ModerateAddress *p, ModerateRequest request) {
    if (*p != NULL) {
        (*p)->info = request;
    }
}

// Insert a ModerateNode at the beginning of the list
void moderate_insert(ModerateList *L, ModerateAddress p) {
    if (L->first == NULL) {
        L->first = p;
    } else {
        p->next = L->first;
        L->first = p;
    }
    if (p->info.request_id > L->id_max) {
        L->id_max = p->info.request_id;
    }
}

// Display the list starting from node p
void moderate_tampil_list(ModerateAddress p) {
    while (p != NULL) {
        printf("Request ID: %d, User ID: %d, Board ID: %d, Approved: %s\n",
               p->info.request_id, p->info.user_id, p->info.board_id,
               p->info.is_approved ? "Yes" : "No");
        p = p->next;
    }
}

// Search for a ModerateNode by request_id
ModerateAddress moderate_search_by_id(ModerateAddress p, Id request_id) {
    while (p != NULL) {
        if (p->info.request_id == request_id) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// Delete a ModerateNode by request_id
void moderate_delete_by_id(ModerateAddress *p, Id request_id, ModerateRequest *X) {
    ModerateAddress prev = NULL;
    ModerateAddress curr = *p;
    while (curr != NULL) {
        if (curr->info.request_id == request_id) {
            if (prev == NULL) {
                *p = curr->next;
            } else {
                prev->next = curr->next;
            }
            *X = curr->info;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// Deallocate all nodes starting from p
void moderate_deallocation(ModerateAddress *p) {
    ModerateAddress curr = *p;
    while (curr != NULL) {
        ModerateAddress temp = curr;
        curr = curr->next;
        free(temp);
    }
    *p = NULL;
}

// Implement missing functions idlist_add and idlist_delete_by_id as wrappers

// Assuming IdList is similar to ModerateList, define a struct for IdList and IdAddress
// typedef struct IdNode *IdAddress;
// typedef struct IdNode {
//     Id info;
//     IdAddress nextId;
// } IdNode;

// typedef struct {
//     IdAddress first;
// } IdList;

// // Add an Id to the IdList (similar to idlist_add)
// void idlist_add(IdList *list, Id id) {
//     IdAddress newNode = (IdAddress)malloc(sizeof(IdNode));
//     if (newNode != NULL) {
//         newNode->info = id;
//         newNode->nextId = list->first;
//         list->first = newNode;
//     }
// }

// // Delete an Id from the IdList by id (similar to idlist_delete_by_id)
// void idlist_delete_by_id(IdList *list, Id id) {
//     IdAddress prev = NULL;
//     IdAddress curr = list->first;
//     while (curr != NULL) {
//         if (curr->info == id) {
//             if (prev == NULL) {
//                 list->first = curr->nextId;
//             } else {
//                 prev->nextId = curr->nextId;
//             }
//             free(curr);
//             return;
//         }
//         prev = curr;
//         curr = curr->nextId;
//     }
// }
