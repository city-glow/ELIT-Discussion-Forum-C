#ifndef USER_C
#define USER_C
#include "../../include/vote/vote.h"
// #include <string.h>
// #include <stdio.h>

void create_vote(Vote *vote, Id user_id, Id target_id, VoteTargetType target_type, bool is_upvote) {
    vote->is_upvote = is_upvote;
    vote->user_id = user_id;
    vote->target_id = target_id;
    vote->target_type = target_type;
}

#endif
