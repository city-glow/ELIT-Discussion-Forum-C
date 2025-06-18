#ifndef BOARD_H
#define BOARD_H
#include "../moderateThis/moderateThisQueue.h"
#include "../moderateThis/moderateThisList.h"
#include "../post/postList.h"
#define MAX_TITLE 100

typedef struct {
  Id id;
  Id owner_id;
  ModerateQueue queue;
  char title[MAX_TITLE + 1];
} Board;

void create_board(Board *board, Id owner_id, char *title);
void board_add_moderate_request(Board *board, Id request_id, Id user_id);
bool board_approve_moderate_request(Board *board, Id request_id,
                                    ModerateList *moderateList, PostList *postList);
bool board_reject_moderate_request(Board *board, Id request_id, PostList *postList);
void board_view_pending_requests(Board *board, ModerateList *moderateList);

#endif
