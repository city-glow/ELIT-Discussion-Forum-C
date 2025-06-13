#include "../../include/moderateThis/moderateThisQueue.h"
#include <stdlib.h>
#include <stdio.h>

// Create an empty queue
void moderate_queue_create(ModerateQueue *Q) {
    Q->front = NULL;
    Q->rear = NULL;
}

// Check if the queue is empty
bool moderate_queue_is_empty(ModerateQueue Q) {
    return (Q.front == NULL);
}

// Enqueue a ModerateRequest to the queue
void moderate_queue_enqueue(ModerateQueue *Q, ModerateRequest request) {
    ModerateQueueAddress newNode = (ModerateQueueAddress)malloc(sizeof(ModerateQueueElmt));
    if (newNode != NULL) {
        newNode->info = request;
        newNode->next = NULL;
        if (Q->rear == NULL) {
            Q->front = newNode;
            Q->rear = newNode;
        } else {
            Q->rear->next = newNode;
            Q->rear = newNode;
        }
    }
}

// Dequeue a ModerateRequest from the queue
bool moderate_queue_dequeue(ModerateQueue *Q, ModerateRequest *request) {
    if (moderate_queue_is_empty(*Q)) {
        return false;
    }
    ModerateQueueAddress temp = Q->front;
    *request = temp->info;
    Q->front = temp->next;
    if (Q->front == NULL) {
        Q->rear = NULL;
    }
    free(temp);
    return true;
}

// View all elements in the queue
void moderate_queue_view(ModerateQueue Q) {
    ModerateQueueAddress current = Q.front;
    while (current != NULL) {
        printf("Request ID: %d, User ID: %d, Board ID: %d, Approved: %s\n",
               current->info.request_id, current->info.user_id, current->info.board_id,
               current->info.is_approved ? "Yes" : "No");
        current = current->next;
    }
}

// Clear the queue
void moderate_queue_clear(ModerateQueue *Q) {
    ModerateQueueAddress current = Q->front;
    while (current != NULL) {
        ModerateQueueAddress temp = current;
        current = current->next;
        free(temp);
    }
    Q->front = NULL;
    Q->rear = NULL;
}
