#ifndef BOARD_H
#define BOARD_H
#include "../id/idList.h"
#define MAX_TITLE 100

typedef struct {
    Id id;
    Id owner_id;
    IdList queue;
    char title[MAX_TITLE + 1];
} Board;

void create_board(Board *board, Id owner_id, char *title);

#endif
