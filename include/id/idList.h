#ifndef IDLIST_H
#define IDLIST_H
#include "id.h"

typedef struct IdTElmtList *IdAddress;
typedef struct IdTElmtList {
  Id info;
  IdAddress nextId;
} IdElmtList;

typedef struct {
  IdAddress first;
} IdList;

void idlist_add(IdList *list, Id id);
void idlist_delete_by_id(IdList *list, Id target);

#endif