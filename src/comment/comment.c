#ifndef COMMENT_C
#define COMMENT_C
#include "../../include/comment/comment.h"
#include <string.h>

void create_comment(Comment *comment, Id user_id, Id post_id, Id reply_to,
                    char *content){
    comment->content = strdup(content);
    comment->user_id = user_id;
    comment->post_id = post_id;
    comment->reply_to = reply_to;
}

#endif
