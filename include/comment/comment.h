#ifndef COMMENT_H
#define COMMENT_H
#include "../id/id.h"

typedef struct {
  Id id;
  Id user_id;
  Id post_id;
  Id reply_to;
  char *content;
} Comment;

void create_comment(Comment *comment, Id user_id, Id post_id, Id reply_to,
                    char *content);

#endif
