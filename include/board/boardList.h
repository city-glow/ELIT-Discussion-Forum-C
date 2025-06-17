#ifndef BOARDLIST_H
#define BOARDLIST_H
#include "board.h"
#include "../post/postList.h"
#include "../comment/commentTreeList.h"
#include "../vote/voteList.h"
#include "board.h"
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
BoardAddress board_search_by_title(BoardAddress p, char* title);
// void board_delete_by_address(BoardAddress *p, BoardAddress pDel, board *X);
void board_delete_by_id(BoardList *p, Id nilai, Board *X, PostList *list, VoteList *vote_list, CommentTreeList *comment_tree_list);
void board_deallocation(BoardAddress *p);
int board_count(BoardAddress p);
BoardAddress board_balik_list(BoardAddress p);
bool get_board_by_id(BoardList p, Id nilai, Board *get);
int board_insert_and_get_id(BoardList *p, BoardAddress PNew);
bool save_board_list(const BoardList *boardList, const char *filename);
bool load_board_list(BoardList *boardList, const char *filename);

#endif
