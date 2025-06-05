#ifndef POST_H
#define POST_H
#define MAX_TITLE 100
#include <stdbool.h>
#include "../id/id.h"

typedef struct {
    Id id;
    Id user_id;
    Id board_id;
    bool edited;
    char title[MAX_TITLE + 1];
    char *content;
} Post;

void create_post(Post *post, Id user_id, Id board_id, char *title, char*content);

#endif
