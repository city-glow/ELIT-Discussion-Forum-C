#ifndef POSTLIST_H
#define POSTLIST_H
#include "post.h"
#include "../comment/commentTreeList.h"
#include "../vote/voteList.h"
#include <stdbool.h>

typedef struct PostTElmtList *PostAddress;
typedef struct PostTElmtList {
  Post info;
  PostAddress next;
} PostElmtList;
typedef struct {
  PostAddress first;
  Id id_max;
} PostList;

bool post_is_empty(PostAddress p);
void post_create_node(PostAddress *p);
void post_create_list(PostList *p);
void post_isi_node(PostAddress *p, Post nilai);
void post_tampil_list(PostAddress p);
void post_insert(PostList *p, PostAddress PNew);
PostAddress post_search_by_id(PostAddress p, Id nilai);
// void Post_delete_by_address(PostAddress *p, PostAddress pDel, Post *X);
void post_delete_by_id(PostList *p, Id nilai, Post *X, VoteList *vote_list, CommentTreeList *comment_tree_list);
void post_deallocation(PostAddress *p);
void post_delete_all_by_board_id(PostList *list, Id board_id, VoteList *vote_list, CommentTreeList *comment_tree_list);
int post_count(PostAddress p);
PostAddress post_balik_list(PostAddress p);
void save_post_list(PostList *list, const char *filename);
void load_post_list(PostList *list, const char *filename);

#endif
