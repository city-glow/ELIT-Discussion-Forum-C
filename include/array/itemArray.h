#ifndef ITEMARRAY_H
#define ITEMARRAY_H
#include "../comment/commentTreeList.h"
#include "../post/postList.h"
#include "../vote/voteList.h"
#include "item.h"

Item *generate_top_comments_array(CommentTreeList comment_list,
                                  VoteList vote_list, int *count);

int get_vote_sum(VoteList vote_list, Id comment_id, VoteTargetType target_type);
Item *generate_top_posts_array(PostList post_list, VoteList vote_list,
                               int *count, Id board, bool sort_by_new, Id user,
                               const char *search_term);
#endif
