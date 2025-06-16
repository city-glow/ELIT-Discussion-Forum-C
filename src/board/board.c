#ifndef BOARD_C
#define BOARD_C
#include "../../include/board/board.h"
#include "../../include/moderateThis/moderateThisQueue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Helper function to remove a request by id from ModerateQueue
static bool moderate_queue_remove_by_id(ModerateQueue *Q, Id request_id) {
    if (moderate_queue_is_empty(*Q)) {
        return false;
    }
    ModerateQueueAddress prev = NULL;
    ModerateQueueAddress curr = Q->front;
    while (curr != NULL) {
        if (curr->info.request_id == request_id) {
            if (prev == NULL) {
                Q->front = curr->next;
            } else {
                prev->next = curr->next;
            }
            if (curr == Q->rear) {
                Q->rear = prev;
            }
            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

void create_board(Board *board, Id owner_id, char *title) {
    strcpy(board->title, title);
    board->owner_id = owner_id;
    moderate_queue_create(&board->queue);
}

bool board_is_moderator(Board *board, Id user_id) {
    return board->owner_id == user_id;
}

void board_add_moderate_request(Board *board, Id request_id) {
    ModerateRequest request = {0};
    request.request_id = request_id;
    request.user_id = 0; // or set appropriately
    request.board_id = board->id;
    request.is_approved = false;
    moderate_queue_enqueue(&board->queue, request);
}

// Approve a moderation request by request_id
bool board_approve_moderate_request(Board *board, Id request_id, ModerateList *moderateList) {
    ModerateAddress modNode = moderate_search_by_id(moderateList->first, request_id);
    if (modNode == NULL) {
        return false; // Request not found
    }
    modNode->info.is_approved = true;
    moderate_queue_remove_by_id(&board->queue, request_id);
    return true;
}

// Reject a moderation request by request_id
bool board_reject_moderate_request(Board *board, Id request_id) {
    // Just remove from queue, no approval
    moderate_queue_remove_by_id(&board->queue, request_id);
    return true;
}

// View pending moderation requests for the board
void board_view_pending_requests(Board *board, ModerateList *moderateList) {
    printf("Pending moderation requests for board '%s':\n", board->title);
    ModerateQueueAddress current = board->queue.front;
    while (current != NULL) {
        ModerateAddress modNode = moderate_search_by_id(moderateList->first, current->info.request_id);
        if (modNode != NULL && !modNode->info.is_approved) {
            printf("Request ID: %d, User ID: %d\n", modNode->info.request_id, modNode->info.user_id);
        }
        current = current->next;
    }
}

#endif
