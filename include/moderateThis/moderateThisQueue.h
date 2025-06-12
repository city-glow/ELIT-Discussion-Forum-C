#ifndef MODERATETHISQUEUE_H
#define MODERATETHISQUEUE_H

#include <stdbool.h>
#include "moderateThis.h"

typedef struct ModerateQueueNode *ModerateQueueAddress;
typedef struct ModerateQueueNode {
    ModerateRequest info;
    ModerateQueueAddress next;
} ModerateQueueElmt;

typedef struct {
    ModerateQueueAddress front, rear;
} ModerateQueue;

void moderate_queue_create(ModerateQueue *Q);
bool moderate_queue_is_empty(ModerateQueue Q);
void moderate_queue_enqueue(ModerateQueue *Q, ModerateRequest request);
bool moderate_queue_dequeue(ModerateQueue *Q, ModerateRequest *request);
void moderate_queue_view(ModerateQueue Q);
void moderate_queue_clear(ModerateQueue *Q);

#endif
