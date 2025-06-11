#ifndef ITEMARRAY_H
#define ITEMARRAY_H
#include "../comment/commentTreeList.h"
#include "../vote/voteList.h"
#include "item.h"

Item *generate_top_comments_array(CommentTreeList comment_list,
                                  VoteList vote_list, int *count);

int get_comment_vote_sum(VoteList vote_list, Id comment_id);
#endif
