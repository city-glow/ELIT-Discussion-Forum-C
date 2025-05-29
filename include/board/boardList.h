#ifndef BOARDLIST_H
#define BOARDLIST_H
#include "board.h"
#include <stdbool.h>

typedef struct BoardTElmtList *BoardAddress;
typedef struct BoardTElmtList {
  Board info;
  BoardAddress next;
} BoardElmtList;
typedef struct {
  BoardAddress first;
  Id id_max;
} BoardList;

bool board_is_empty(BoardAddress p);
void board_create_node(BoardAddress *p);
void board_create_list(BoardList *p);
void board_isi_node(BoardAddress *p, Board nilai);
void board_tampil_list(BoardAddress p);
void board_insert(BoardList *p, BoardAddress PNew);
BoardAddress board_search_by_id(BoardAddress p, Id nilai);
// void board_delete_by_address(BoardAddress *p, BoardAddress pDel, board *X);
void board_delete_by_id(BoardAddress *p, Id nilai, Board *X);
void board_deallocation(BoardAddress *p);
int board_count(BoardAddress p);
BoardAddress board_balik_list(BoardAddress p);

#endif
