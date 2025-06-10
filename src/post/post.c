#ifndef POST_C
#define POST_C
#include "../../include/post/post.h"
#include <string.h>

void create_post(Post *post, Id user_id, Id board_id, char* title, char *content) {
    strcpy(post->title, title);
    post->content = strdup(content);
    post->user_id = user_id;
    post->board_id = board_id;
    post->edited = false;
    post->approved = false;
}

#endif
