#ifndef MODERATETHISLIST_H
#define MODERATETHISLIST_H

#include <stdbool.h>
#include "moderateThis.h"

typedef struct ModerateNode *ModerateAddress;
typedef struct ModerateNode {
    ModerateRequest info;
    ModerateAddress next;
} ModerateElmtList;

typedef struct {
    ModerateAddress first;
    int id_max;
} ModerateList;

void moderate_create_list(ModerateList *L);
void moderate_create_node(ModerateAddress *p);
void moderate_isi_node(ModerateAddress *p, ModerateRequest request);
void moderate_insert(ModerateList *L, ModerateAddress p);
void moderate_tampil_list(ModerateAddress p);
ModerateAddress moderate_search_by_id(ModerateAddress p, Id request_id);
void moderate_delete_by_id(ModerateAddress *p, Id request_id, ModerateRequest *X);
void moderate_deallocation(ModerateAddress *p);

#endif
