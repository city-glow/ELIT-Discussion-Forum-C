#ifndef BOARD_C
#define BOARD_C
#include "../../include/board/board.h"
#include <string.h>
#include <stdio.h>

void create_board(Board *board, Id owner_id, char *title) {
    strcpy(board->title, title);
    board->owner_id = owner_id;
    board->queue.first = NULL;
}

#endif
