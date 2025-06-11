#ifndef ITEM_H
#define ITEM_H
#include "../id/id.h"
#include "../comment/comment.h"
#include "../post/post.h"
#include "../board/board.h"
typedef enum {
    ITEM_TYPE_POST,
    ITEM_TYPE_COMMENT,
    ITEM_TYPE_BOARD
} ItemType;

typedef union {
    Comment c;
    Post p;
    Board b;
} ItemValue;

typedef struct {
    ItemValue info;
    ItemType type;
    Id id; // Store ID for lookup efficiency
    int vote_sum;
} Item;
#endif
