#ifndef BOARD_C
#define BOARD_C
#include "../../include/board/board.h"
#include "../../include/moderateThis/moderateThisList.h"
#include "../../include/moderateThis/moderateThisQueue.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void create_board(Board *board, Id owner_id, char *title) {
    strcpy(board->title, title);
    board->owner_id = owner_id;
    board->queue.first = NULL;
}

bool board_is_moderator(Board *board, Id user_id) {
    return board->owner_id == user_id;
}

void board_add_moderate_request(Board *board, Id request_id) {
    idlist_add(&board->queue, request_id);
}

// Approve a moderation request by request_id
bool board_approve_moderate_request(Board *board, Id request_id, ModerateList *moderateList) {
    ModerateAddress modNode = moderate_search_by_id(moderateList->first, request_id);
    if (modNode == NULL) {
        return false; // Request not found
    }
    modNode->info.is_approved = true;
    idlist_delete_by_id(&board->queue, request_id);
    return true;
}

// Reject a moderation request by request_id
bool board_reject_moderate_request(Board *board, Id request_id) {
    // Just remove from queue, no approval
    idlist_delete_by_id(&board->queue, request_id);
    return true;
}

// View pending moderation requests for the board
void board_view_pending_requests(Board *board, ModerateList *moderateList) {
    printf("Pending moderation requests for board '%s':\n", board->title);
    IdAddress current = board->queue.first;
    while (current != NULL) {
        ModerateAddress modNode = moderate_search_by_id(moderateList->first, current->info);
        if (modNode != NULL && !modNode->info.is_approved) {
            printf("Request ID: %d, User ID: %d\n", modNode->info.request_id, modNode->info.user_id);
        }
        current = current->nextId;
    }
}

#endif
