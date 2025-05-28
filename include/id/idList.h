#ifndef IDLIST_H
#define IDLIST_H
#include "id.h"

typedef struct IdTElmtList *IdAddress;
typedef struct IdTElmtList {
  Id info;
  IdAddress nextId;
} IdElmtList;
typedef IdAddress IdList;

#endif
