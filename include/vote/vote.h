#ifndef VOTE_H
#define VOTE_H
#include "../id/id.h"
#include <stdbool.h>

typedef enum {
    VOTE_TARGET_POST,
    VOTE_TARGET_COMMENT
} VoteTargetType;

typedef struct {
    Id id;
    Id user_id;
    Id target_id;
    VoteTargetType target_type;
    bool is_upvote;
} Vote;

void create_vote(Vote *vote, Id user_id, Id target_id, VoteTargetType target_type, bool is_upvote);

#endif
