#ifndef ITEM_H
#define ITEM_H
#include "../id/id.h"
typedef enum {
    ITEM_TYPE_POST,
    ITEM_TYPE_COMMENT
} ItemType;

typedef struct {
    ItemType type;
    Id id; // Store ID for lookup efficiency
    int vote_sum;
} Item;
#endif
